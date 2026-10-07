#include "mini_fs.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

enum { DIRECT_COUNT=4, INDIRECT_COUNT=64, NAME_CAP=32 };
#define INVALID_BLOCK UINT32_MAX

typedef struct {
    FsNodeType type;
    size_t size_bytes;
    size_t data_blocks;
    uint32_t direct[DIRECT_COUNT];
    uint32_t indirect_block;
    uint32_t indirect[INDIRECT_COUNT];
} FsInode;

typedef struct {
    uint32_t parent;
    uint32_t child;
    char name[NAME_CAP];
} FsDirEntry;

struct MiniFs {
    size_t inode_capacity;
    size_t block_count;
    size_t block_size;
    size_t inode_count;
    size_t allocated_blocks;
    size_t entry_count;
    unsigned char *inode_used;
    unsigned char *block_used;
    FsInode *inodes;
    FsDirEntry *entries;
};

static void inode_init(FsInode *inode,FsNodeType type){
    inode->type=type;
    inode->size_bytes=0U;
    inode->data_blocks=0U;
    for(size_t i=0;i<DIRECT_COUNT;++i)inode->direct[i]=INVALID_BLOCK;
    inode->indirect_block=INVALID_BLOCK;
    for(size_t i=0;i<INDIRECT_COUNT;++i)inode->indirect[i]=INVALID_BLOCK;
}

MiniFs *fs_create(size_t inode_capacity,size_t block_count,size_t block_size){
    if(inode_capacity==0U||block_count==0U||block_size==0U||
       inode_capacity>UINT32_MAX||block_count>UINT32_MAX||
       inode_capacity>SIZE_MAX/sizeof(FsInode)||
       inode_capacity>SIZE_MAX/sizeof(FsDirEntry))return NULL;

    MiniFs *fs=calloc(1,sizeof(*fs));
    if(!fs)return NULL;

    fs->inode_used=calloc(inode_capacity,1U);
    fs->block_used=calloc(block_count,1U);
    fs->inodes=calloc(inode_capacity,sizeof(*fs->inodes));
    fs->entries=calloc(inode_capacity,sizeof(*fs->entries));
    if(!fs->inode_used||!fs->block_used||!fs->inodes||!fs->entries){
        fs_free(fs);
        return NULL;
    }

    fs->inode_capacity=inode_capacity;
    fs->block_count=block_count;
    fs->block_size=block_size;
    fs->inode_used[0]=1U;
    inode_init(&fs->inodes[0],FS_NODE_DIR);
    fs->inode_count=1U;
    return fs;
}

void fs_free(MiniFs *fs){
    if(!fs)return;
    free(fs->inode_used);
    free(fs->block_used);
    free(fs->inodes);
    free(fs->entries);
    free(fs);
}

static bool valid_inode(const MiniFs *fs,uint32_t inode){
    return fs&&inode<fs->inode_capacity&&fs->inode_used[inode];
}

bool fs_lookup(const MiniFs *fs,uint32_t parent,const char *name,uint32_t *out_inode){
    if(!fs||!name||!out_inode||!valid_inode(fs,parent)||
       fs->inodes[parent].type!=FS_NODE_DIR)return false;

    for(size_t i=0;i<fs->entry_count;++i){
        if(fs->entries[i].parent==parent&&strcmp(fs->entries[i].name,name)==0){
            *out_inode=fs->entries[i].child;
            return true;
        }
    }
    return false;
}

static bool create_node(MiniFs *fs,uint32_t parent,const char *name,
                        FsNodeType type,uint32_t *out_inode){
    if(!fs||!name||!out_inode||!valid_inode(fs,parent)||
       fs->inodes[parent].type!=FS_NODE_DIR)return false;

    size_t len=strlen(name);
    if(len==0U||len>=NAME_CAP||fs->entry_count>=fs->inode_capacity-1U)return false;

    uint32_t existing=0U;
    if(fs_lookup(fs,parent,name,&existing))return false;

    size_t idx=1U;
    while(idx<fs->inode_capacity&&fs->inode_used[idx])++idx;
    if(idx==fs->inode_capacity)return false;

    fs->inode_used[idx]=1U;
    inode_init(&fs->inodes[idx],type);
    ++fs->inode_count;

    FsDirEntry *entry=&fs->entries[fs->entry_count++];
    entry->parent=parent;
    entry->child=(uint32_t)idx;
    memcpy(entry->name,name,len+1U);
    *out_inode=(uint32_t)idx;
    return true;
}

bool fs_mkdir(MiniFs *fs,uint32_t parent,const char *name,uint32_t *out_inode){
    return create_node(fs,parent,name,FS_NODE_DIR,out_inode);
}

bool fs_create_file(MiniFs *fs,uint32_t parent,const char *name,uint32_t *out_inode){
    return create_node(fs,parent,name,FS_NODE_FILE,out_inode);
}

static size_t blocks_for_size(size_t size,size_t block_size){
    if(size==0U)return 0U;
    return size/block_size+(size%block_size!=0U);
}

static uint32_t *mapping_slot(FsInode *inode,size_t logical){
    if(logical<DIRECT_COUNT)return &inode->direct[logical];
    logical-=DIRECT_COUNT;
    if(logical>=INDIRECT_COUNT)return NULL;
    return &inode->indirect[logical];
}

bool fs_resize_file(MiniFs *fs,uint32_t inode_id,size_t new_size){
    if(!valid_inode(fs,inode_id)||fs->inodes[inode_id].type!=FS_NODE_FILE)return false;
    FsInode *inode=&fs->inodes[inode_id];

    size_t target=blocks_for_size(new_size,fs->block_size);
    if(target>DIRECT_COUNT+INDIRECT_COUNT)return false;

    if(target>inode->data_blocks){
        size_t add_data=target-inode->data_blocks;
        size_t add_meta=(inode->data_blocks<=DIRECT_COUNT&&
                         target>DIRECT_COUNT&&
                         inode->indirect_block==INVALID_BLOCK)?1U:0U;
        size_t need=add_data+add_meta;
        uint32_t chosen[DIRECT_COUNT+INDIRECT_COUNT+1U];
        size_t found=0U;

        for(size_t b=0;b<fs->block_count&&found<need;++b)
            if(!fs->block_used[b])chosen[found++]=(uint32_t)b;
        if(found<need)return false;

        size_t cursor=0U;
        if(add_meta){
            uint32_t block=chosen[cursor++];
            fs->block_used[block]=1U;
            ++fs->allocated_blocks;
            inode->indirect_block=block;
        }

        for(size_t logical=inode->data_blocks;logical<target;++logical){
            uint32_t *slot=mapping_slot(inode,logical);
            if(!slot)return false;
            uint32_t block=chosen[cursor++];
            fs->block_used[block]=1U;
            ++fs->allocated_blocks;
            *slot=block;
        }
        inode->data_blocks=target;
    }else if(target<inode->data_blocks){
        for(size_t logical=inode->data_blocks;logical>target;--logical){
            uint32_t *slot=mapping_slot(inode,logical-1U);
            if(!slot||*slot==INVALID_BLOCK)return false;
            fs->block_used[*slot]=0U;
            --fs->allocated_blocks;
            *slot=INVALID_BLOCK;
        }
        inode->data_blocks=target;

        if(target<=DIRECT_COUNT&&inode->indirect_block!=INVALID_BLOCK){
            fs->block_used[inode->indirect_block]=0U;
            --fs->allocated_blocks;
            inode->indirect_block=INVALID_BLOCK;
            for(size_t i=0;i<INDIRECT_COUNT;++i)inode->indirect[i]=INVALID_BLOCK;
        }
    }

    inode->size_bytes=new_size;
    return true;
}

bool fs_get_file_block(const MiniFs *fs,uint32_t inode_id,size_t logical,
                       uint32_t *out_physical){
    if(!fs||!out_physical||!valid_inode(fs,inode_id)||
       fs->inodes[inode_id].type!=FS_NODE_FILE)return false;
    const FsInode *inode=&fs->inodes[inode_id];
    if(logical>=inode->data_blocks)return false;

    uint32_t block=logical<DIRECT_COUNT
        ?inode->direct[logical]
        :inode->indirect[logical-DIRECT_COUNT];
    if(block==INVALID_BLOCK||block>=fs->block_count)return false;
    *out_physical=block;
    return true;
}

size_t fs_file_size(const MiniFs *fs,uint32_t inode){
    return valid_inode(fs,inode)&&fs->inodes[inode].type==FS_NODE_FILE
        ?fs->inodes[inode].size_bytes:0U;
}
size_t fs_file_block_count(const MiniFs *fs,uint32_t inode){
    return valid_inode(fs,inode)&&fs->inodes[inode].type==FS_NODE_FILE
        ?fs->inodes[inode].data_blocks:0U;
}
size_t fs_allocated_inode_count(const MiniFs *fs){return fs?fs->inode_count:0U;}
size_t fs_allocated_block_count(const MiniFs *fs){return fs?fs->allocated_blocks:0U;}
size_t fs_block_size(const MiniFs *fs){return fs?fs->block_size:0U;}

bool fs_validate(const MiniFs *fs){
    if(!fs||!fs->inode_used||!fs->block_used||!fs->inodes||!fs->entries||
       fs->inode_capacity==0U||fs->block_count==0U||fs->block_size==0U||
       !fs->inode_used[0]||fs->inodes[0].type!=FS_NODE_DIR)return false;

    unsigned char *seen_blocks=calloc(fs->block_count,1U);
    unsigned char *seen_children=calloc(fs->inode_capacity,1U);
    if(!seen_blocks||!seen_children){
        free(seen_blocks);
        free(seen_children);
        return false;
    }

    size_t inode_count=0U,block_count=0U;
    bool ok=true;

    for(size_t i=0;i<fs->inode_capacity&&ok;++i){
        if(!fs->inode_used[i])continue;
        ++inode_count;
        const FsInode *inode=&fs->inodes[i];

        if(inode->type!=FS_NODE_FILE&&inode->type!=FS_NODE_DIR){ok=false;break;}
        if(inode->type==FS_NODE_DIR){
            if(inode->size_bytes!=0U||inode->data_blocks!=0U||
               inode->indirect_block!=INVALID_BLOCK){ok=false;break;}
            continue;
        }

        size_t expected=blocks_for_size(inode->size_bytes,fs->block_size);
        if(expected!=inode->data_blocks||
           inode->data_blocks>DIRECT_COUNT+INDIRECT_COUNT){ok=false;break;}

        for(size_t d=0;d<DIRECT_COUNT;++d){
            bool should=d<inode->data_blocks;
            if(should){
                uint32_t b=inode->direct[d];
                if(b==INVALID_BLOCK||b>=fs->block_count||seen_blocks[b]||!fs->block_used[b]){
                    ok=false;break;
                }
                seen_blocks[b]=1U;++block_count;
            }else if(inode->direct[d]!=INVALID_BLOCK){
                ok=false;break;
            }
        }
        if(!ok)break;

        size_t indirect_data=inode->data_blocks>DIRECT_COUNT
            ?inode->data_blocks-DIRECT_COUNT:0U;
        if(indirect_data>0U){
            uint32_t meta=inode->indirect_block;
            if(meta==INVALID_BLOCK||meta>=fs->block_count||
               seen_blocks[meta]||!fs->block_used[meta]){ok=false;break;}
            seen_blocks[meta]=1U;++block_count;
        }else if(inode->indirect_block!=INVALID_BLOCK){
            ok=false;break;
        }

        for(size_t d=0;d<INDIRECT_COUNT;++d){
            bool should=d<indirect_data;
            if(should){
                uint32_t b=inode->indirect[d];
                if(b==INVALID_BLOCK||b>=fs->block_count||seen_blocks[b]||!fs->block_used[b]){
                    ok=false;break;
                }
                seen_blocks[b]=1U;++block_count;
            }else if(inode->indirect[d]!=INVALID_BLOCK){
                ok=false;break;
            }
        }
    }

    for(size_t i=0;i<fs->entry_count&&ok;++i){
        const FsDirEntry *e=&fs->entries[i];
        if(e->parent>=fs->inode_capacity||e->child>=fs->inode_capacity||
           !fs->inode_used[e->parent]||!fs->inode_used[e->child]||
           fs->inodes[e->parent].type!=FS_NODE_DIR||
           e->child==0U||e->name[0]=='\0'||seen_children[e->child]){
            ok=false;break;
        }
        seen_children[e->child]=1U;
        for(size_t j=i+1U;j<fs->entry_count;++j){
            if(fs->entries[j].parent==e->parent&&
               strcmp(fs->entries[j].name,e->name)==0){
                ok=false;break;
            }
        }
    }

    if(ok){
        for(size_t i=1U;i<fs->inode_capacity;++i)
            if(fs->inode_used[i]&&!seen_children[i]){ok=false;break;}
    }

    if(ok){
        for(size_t b=0;b<fs->block_count;++b){
            if((seen_blocks[b]!=0U)!=(fs->block_used[b]!=0U)){ok=false;break;}
        }
    }

    ok=ok&&inode_count==fs->inode_count&&
       block_count==fs->allocated_blocks&&
       fs->entry_count+1U==fs->inode_count;

    free(seen_blocks);
    free(seen_children);
    return ok;
}
