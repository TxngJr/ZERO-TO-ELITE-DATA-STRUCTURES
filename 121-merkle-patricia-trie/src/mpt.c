#include "mpt.h"
#include "merkle_tree.h"
#include <stdlib.h>
#include <string.h>

enum { MPT_KEY_NIBBLES = MPT_KEY_BYTES * 2U };

typedef enum { NODE_LEAF=1,NODE_EXT=2,NODE_BRANCH=3 } NodeType;

typedef struct MptNode MptNode;
struct MptNode {
    NodeType type;
    union {
        struct { unsigned char *path; size_t len; uint64_t value; } leaf;
        struct { unsigned char *path; size_t len; MptNode *child; } ext;
        struct { MptNode *child[16]; } branch;
    } as;
};

struct Mpt {
    MptNode *root;
    size_t size;
};

static void key_to_nibbles(const unsigned char key[MPT_KEY_BYTES],
                           unsigned char out[MPT_KEY_NIBBLES]) {
    for(size_t i=0;i<MPT_KEY_BYTES;++i){
        out[i*2U]=(unsigned char)(key[i]>>4U);
        out[i*2U+1U]=(unsigned char)(key[i]&0x0fU);
    }
}

static MptNode *node_new(NodeType type){
    MptNode*n=calloc(1,sizeof(*n));
    if(n)n->type=type;
    return n;
}

static unsigned char *path_dup(const unsigned char *path,size_t len){
    if(len==0U)return NULL;
    unsigned char*p=malloc(len);
    if(!p)return NULL;
    memcpy(p,path,len);
    return p;
}

static MptNode *leaf_new(const unsigned char *path,size_t len,uint64_t value){
    MptNode*n=node_new(NODE_LEAF);if(!n)return NULL;
    n->as.leaf.path=path_dup(path,len);
    if(len&& !n->as.leaf.path){free(n);return NULL;}
    n->as.leaf.len=len;n->as.leaf.value=value;return n;
}

static MptNode *ext_new(const unsigned char *path,size_t len,MptNode*child){
    if(len==0U||!child)return NULL;
    MptNode*n=node_new(NODE_EXT);if(!n)return NULL;
    n->as.ext.path=path_dup(path,len);
    if(!n->as.ext.path){free(n);return NULL;}
    n->as.ext.len=len;n->as.ext.child=child;return n;
}

static MptNode *branch_new(void){return node_new(NODE_BRANCH);}

static void node_free(MptNode*n){
    if(!n)return;
    if(n->type==NODE_LEAF){
        free(n->as.leaf.path);
    }else if(n->type==NODE_EXT){
        free(n->as.ext.path);node_free(n->as.ext.child);
    }else if(n->type==NODE_BRANCH){
        for(size_t i=0;i<16U;++i)node_free(n->as.branch.child[i]);
    }
    free(n);
}

Mpt *mpt_create(void){return calloc(1,sizeof(Mpt));}
void mpt_free(Mpt*t){if(!t)return;node_free(t->root);free(t);}

static size_t common_prefix(const unsigned char*a,size_t alen,
                            const unsigned char*b,size_t blen){
    size_t n=alen<blen?alen:blen,i=0U;
    while(i<n&&a[i]==b[i])++i;
    return i;
}

static bool attach_suffix(MptNode *branch,const unsigned char *path,size_t len,
                          uint64_t value,MptNode *existing,bool existing_mode){
    if(len==0U)return false;
    unsigned nib=path[0];
    if(existing_mode){
        if(len==1U){branch->as.branch.child[nib]=existing;return true;}
        MptNode*e=ext_new(path+1U,len-1U,existing);
        if(!e)return false;
        branch->as.branch.child[nib]=e;
        return true;
    }
    MptNode*l=leaf_new(path+1U,len-1U,value);
    if(!l)return false;
    branch->as.branch.child[nib]=l;
    return true;
}

static void cleanup_split_branch(MptNode *branch,unsigned old_nibble,
                                 MptNode *old_child){
    if(!branch)return;
    MptNode *arm=branch->as.branch.child[old_nibble];
    if(arm==old_child){
        branch->as.branch.child[old_nibble]=NULL;
    }else if(arm&&arm->type==NODE_EXT&&arm->as.ext.child==old_child){
        arm->as.ext.child=NULL;
        node_free(arm);
        branch->as.branch.child[old_nibble]=NULL;
    }
    node_free(branch);
}

static bool insert_node(MptNode **link,const unsigned char *key,size_t remaining,
                        uint64_t value,bool *inserted){
    MptNode*n=*link;
    if(!n){
        *link=leaf_new(key,remaining,value);
        if(!*link)return false;
        *inserted=true;return true;
    }

    if(n->type==NODE_LEAF){
        size_t cp=common_prefix(n->as.leaf.path,n->as.leaf.len,key,remaining);
        if(cp==remaining&&cp==n->as.leaf.len){
            n->as.leaf.value=value;*inserted=false;return true;
        }
        if(cp>=remaining||cp>=n->as.leaf.len)return false;

        MptNode*branch=branch_new();if(!branch)return false;
        unsigned char *old_path=n->as.leaf.path;
        size_t old_len=n->as.leaf.len;
        uint64_t old_value=n->as.leaf.value;

        MptNode*old_leaf=leaf_new(old_path+cp+1U,old_len-cp-1U,old_value);
        MptNode*new_leaf=leaf_new(key+cp+1U,remaining-cp-1U,value);
        if(!old_leaf||!new_leaf){node_free(old_leaf);node_free(new_leaf);free(branch);return false;}
        branch->as.branch.child[old_path[cp]]=old_leaf;
        branch->as.branch.child[key[cp]]=new_leaf;

        MptNode*replacement=branch;
        if(cp){
            replacement=ext_new(key,cp,branch);
            if(!replacement){
                cleanup_split_branch(branch,old_path[cp],old_child);
                return false;
            }
        }
        free(old_path);free(n);*link=replacement;*inserted=true;return true;
    }

    if(n->type==NODE_EXT){
        size_t cp=common_prefix(n->as.ext.path,n->as.ext.len,key,remaining);
        if(cp==n->as.ext.len)
            return insert_node(&n->as.ext.child,key+cp,remaining-cp,value,inserted);

        if(cp>=remaining)return false;
        MptNode*branch=branch_new();if(!branch)return false;

        unsigned char *old_path=n->as.ext.path;
        size_t old_len=n->as.ext.len;
        MptNode*old_child=n->as.ext.child;

        if(!attach_suffix(branch,old_path+cp,old_len-cp,0U,old_child,true)){
            free(branch);return false;
        }
        if(!attach_suffix(branch,key+cp,remaining-cp,value,NULL,false)){
            cleanup_split_branch(branch,old_path[cp],old_child);
            return false;
        }

        MptNode*replacement=branch;
        if(cp){
            replacement=ext_new(key,cp,branch);
            if(!replacement){node_free(branch);return false;}
        }
        free(old_path);free(n);*link=replacement;*inserted=true;return true;
    }

    if(n->type==NODE_BRANCH){
        if(remaining==0U)return false;
        unsigned nib=key[0];
        return insert_node(&n->as.branch.child[nib],key+1U,remaining-1U,value,inserted);
    }
    return false;
}

bool mpt_put(Mpt*t,const unsigned char key[MPT_KEY_BYTES],uint64_t value,bool*out_inserted){
    if(!t||!key)return false;
    unsigned char nibbles[MPT_KEY_NIBBLES];key_to_nibbles(key,nibbles);
    bool inserted=false;
    if(!insert_node(&t->root,nibbles,MPT_KEY_NIBBLES,value,&inserted))return false;
    if(inserted)++t->size;
    if(out_inserted)*out_inserted=inserted;
    return true;
}

static bool lookup_node(const MptNode*n,const unsigned char*key,size_t remaining,
                        uint64_t*out_value,bool*out_found){
    if(!n){*out_found=false;return true;}
    if(n->type==NODE_LEAF){
        if(n->as.leaf.len==remaining &&
           (remaining==0U||memcmp(n->as.leaf.path,key,remaining)==0)){
            *out_value=n->as.leaf.value;*out_found=true;
        }else *out_found=false;
        return true;
    }
    if(n->type==NODE_EXT){
        if(remaining<n->as.ext.len ||
           memcmp(n->as.ext.path,key,n->as.ext.len)!=0){
            *out_found=false;return true;
        }
        return lookup_node(n->as.ext.child,key+n->as.ext.len,
                           remaining-n->as.ext.len,out_value,out_found);
    }
    if(n->type==NODE_BRANCH){
        if(remaining==0U){*out_found=false;return true;}
        return lookup_node(n->as.branch.child[key[0]],key+1U,remaining-1U,
                           out_value,out_found);
    }
    return false;
}

bool mpt_get(const Mpt*t,const unsigned char key[MPT_KEY_BYTES],
             uint64_t*out_value,bool*out_found){
    if(!t||!key||!out_value||!out_found)return false;
    unsigned char nibbles[MPT_KEY_NIBBLES];key_to_nibbles(key,nibbles);
    return lookup_node(t->root,nibbles,MPT_KEY_NIBBLES,out_value,out_found);
}

static void be16(unsigned char out[2],size_t v){
    out[0]=(unsigned char)(v>>8U);out[1]=(unsigned char)v;
}
static void be64(unsigned char out[8],uint64_t v){
    for(unsigned i=0;i<8U;++i)out[7U-i]=(unsigned char)(v>>(i*8U));
}

static bool node_hash(const MptNode*n,unsigned char out[32]){
    if(!n){memset(out,0,32U);return true;}
    if(n->type==NODE_LEAF){
        size_t len=n->as.leaf.len;
        if(len>64U)return false;
        unsigned char buf[1U+2U+64U+8U];
        size_t p=0U;buf[p++]=0x20U;be16(buf+p,len);p+=2U;
        if(len){memcpy(buf+p,n->as.leaf.path,len);p+=len;}
        be64(buf+p,n->as.leaf.value);p+=8U;
        merkle_sha256(buf,p,out);return true;
    }
    if(n->type==NODE_EXT){
        if(n->as.ext.len==0U||n->as.ext.len>64U||!n->as.ext.child)return false;
        unsigned char child[32];
        if(!node_hash(n->as.ext.child,child))return false;
        unsigned char buf[1U+2U+64U+32U];
        size_t p=0U;buf[p++]=0x21U;be16(buf+p,n->as.ext.len);p+=2U;
        memcpy(buf+p,n->as.ext.path,n->as.ext.len);p+=n->as.ext.len;
        memcpy(buf+p,child,32U);p+=32U;
        merkle_sha256(buf,p,out);return true;
    }
    if(n->type==NODE_BRANCH){
        unsigned char buf[1U+16U*32U];buf[0]=0x22U;
        for(size_t i=0;i<16U;++i)
            if(!node_hash(n->as.branch.child[i],buf+1U+i*32U))return false;
        merkle_sha256(buf,sizeof(buf),out);return true;
    }
    return false;
}

bool mpt_root(const Mpt*t,unsigned char out[32]){
    if(!t||!out)return false;
    return node_hash(t->root,out);
}
size_t mpt_size(const Mpt*t){return t?t->size:0U;}

static bool validate_node(const MptNode*n,size_t remaining,size_t*nodes,size_t*leaves){
    if(!n)return false;
    ++*nodes;
    if(n->type==NODE_LEAF){
        if(n->as.leaf.len!=remaining)return false;
        for(size_t i=0;i<n->as.leaf.len;++i)if(n->as.leaf.path[i]>15U)return false;
        ++*leaves;return true;
    }
    if(n->type==NODE_EXT){
        if(n->as.ext.len==0U||n->as.ext.len>=remaining||!n->as.ext.child||
           n->as.ext.child->type==NODE_EXT)return false;
        for(size_t i=0;i<n->as.ext.len;++i)if(n->as.ext.path[i]>15U)return false;
        return validate_node(n->as.ext.child,remaining-n->as.ext.len,nodes,leaves);
    }
    if(n->type==NODE_BRANCH){
        if(remaining==0U)return false;
        size_t children=0U;
        for(size_t i=0;i<16U;++i)if(n->as.branch.child[i]){
            ++children;
            if(!validate_node(n->as.branch.child[i],remaining-1U,nodes,leaves))return false;
        }
        return children>=2U;
    }
    return false;
}

size_t mpt_node_count(const Mpt*t){
    if(!t||!t->root)return 0U;
    size_t nodes=0U,leaves=0U;
    if(!validate_node(t->root,MPT_KEY_NIBBLES,&nodes,&leaves))return 0U;
    return nodes;
}

bool mpt_validate(const Mpt*t){
    if(!t)return false;
    if(!t->root)return t->size==0U;
    size_t nodes=0U,leaves=0U;
    if(!validate_node(t->root,MPT_KEY_NIBBLES,&nodes,&leaves))return false;
    if(leaves!=t->size)return false;
    unsigned char root[32];
    return node_hash(t->root,root);
}
