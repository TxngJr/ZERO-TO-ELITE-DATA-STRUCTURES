#ifndef MINI_FS_H
#define MINI_FS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct MiniFs MiniFs;

typedef enum {
    FS_NODE_FILE=1,
    FS_NODE_DIR=2
} FsNodeType;

MiniFs *fs_create(size_t inode_capacity,size_t block_count,size_t block_size);
void fs_free(MiniFs *fs);

bool fs_mkdir(MiniFs *fs,uint32_t parent,const char *name,uint32_t *out_inode);
bool fs_create_file(MiniFs *fs,uint32_t parent,const char *name,uint32_t *out_inode);
bool fs_lookup(const MiniFs *fs,uint32_t parent,const char *name,uint32_t *out_inode);

bool fs_resize_file(MiniFs *fs,uint32_t inode,size_t new_size);
bool fs_get_file_block(const MiniFs *fs,uint32_t inode,size_t logical_block,
                       uint32_t *out_physical);

size_t fs_file_size(const MiniFs *fs,uint32_t inode);
size_t fs_file_block_count(const MiniFs *fs,uint32_t inode);
size_t fs_allocated_inode_count(const MiniFs *fs);
size_t fs_allocated_block_count(const MiniFs *fs);
size_t fs_block_size(const MiniFs *fs);

bool fs_validate(const MiniFs *fs);

#endif
