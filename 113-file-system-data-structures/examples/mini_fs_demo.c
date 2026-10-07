#include "mini_fs.h"
#include <stdio.h>

int main(void){
    MiniFs *fs=fs_create(64U,256U,4096U);
    if(!fs)return 1;

    uint32_t docs=0U,file=0U;
    if(!fs_mkdir(fs,0U,"docs",&docs))return 2;
    if(!fs_create_file(fs,docs,"notes.txt",&file))return 3;
    if(!fs_resize_file(fs,file,5U*4096U))return 4;

    printf("inode=%u size=%zu data_blocks=%zu allocated_blocks=%zu\n",
           file,fs_file_size(fs,file),fs_file_block_count(fs,file),
           fs_allocated_block_count(fs));

    fs_free(fs);
    return 0;
}
