#include "mini_fs.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

enum { DIRS=10, FILES=1000 };

int main(void){
    MiniFs *fs=fs_create(2048U,20000U,4096U);
    assert(fs&&fs_validate(fs));

    uint32_t dirs[DIRS];
    for(size_t i=0;i<DIRS;++i){
        char name[32];
        snprintf(name,sizeof(name),"dir%zu",i);
        assert(fs_mkdir(fs,0U,name,&dirs[i]));
    }

    uint32_t files[FILES];
    size_t expected_blocks=0U;
    for(size_t i=0;i<FILES;++i){
        char name[32];
        snprintf(name,sizeof(name),"file%04zu",i);
        uint32_t parent=dirs[i%DIRS];
        assert(fs_create_file(fs,parent,name,&files[i]));

        size_t blocks=i%21U;
        size_t size=blocks==0U?0U:blocks*4096U-(i%4095U);
        assert(fs_resize_file(fs,files[i],size));
        size_t actual=fs_file_block_count(fs,files[i]);
        assert(actual==blocks);
        expected_blocks+=blocks+(blocks>4U?1U:0U);

        uint32_t looked=0U;
        assert(fs_lookup(fs,parent,name,&looked)&&looked==files[i]);

        for(size_t logical=0;logical<blocks;++logical){
            uint32_t physical=0U;
            assert(fs_get_file_block(fs,files[i],logical,&physical));
            assert(physical<20000U);
        }
    }

    assert(fs_allocated_inode_count(fs)==1U+DIRS+FILES);
    assert(fs_allocated_block_count(fs)==expected_blocks);
    assert(fs_validate(fs));

    size_t before=fs_allocated_block_count(fs);
    for(size_t i=0;i<FILES;i+=2U)assert(fs_resize_file(fs,files[i],0U));
    assert(fs_allocated_block_count(fs)<before);
    assert(fs_validate(fs));

    uint32_t crossing=files[1];
    assert(fs_resize_file(fs,crossing,4U*4096U));
    assert(fs_file_block_count(fs,crossing)==4U);
    size_t blocks_at_four=fs_allocated_block_count(fs);
    assert(fs_resize_file(fs,crossing,5U*4096U));
    assert(fs_allocated_block_count(fs)==blocks_at_four+2U);
    assert(fs_resize_file(fs,crossing,4U*4096U));
    assert(fs_allocated_block_count(fs)==blocks_at_four);

    assert(fs_validate(fs));
    puts("Mini filesystem tests passed");
    fs_free(fs);
    return 0;
}
