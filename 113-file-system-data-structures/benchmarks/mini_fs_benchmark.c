#include "mini_fs.h"
#include <stdio.h>
#include <time.h>

enum { FILES=5000 };

static double elapsed(struct timespec a,struct timespec b){
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1e9;
}

int main(void){
    MiniFs *fs=fs_create(6000U,100000U,4096U);
    if(!fs)return 1;

    uint32_t rootdir=0U;
    if(!fs_mkdir(fs,0U,"bench",&rootdir))return 2;

    struct timespec a,b;
    timespec_get(&a,TIME_UTC);
    for(size_t i=0;i<FILES;++i){
        char name[32];
        snprintf(name,sizeof(name),"f%zu",i);
        uint32_t inode=0U;
        if(!fs_create_file(fs,rootdir,name,&inode))return 3;
        size_t blocks=(i%16U)+1U;
        if(!fs_resize_file(fs,inode,blocks*4096U))return 4;
    }
    timespec_get(&b,TIME_UTC);

    printf("files=%d seconds=%.6f inodes=%zu blocks=%zu\n",
           FILES,elapsed(a,b),fs_allocated_inode_count(fs),
           fs_allocated_block_count(fs));

    fs_free(fs);
    return 0;
}
