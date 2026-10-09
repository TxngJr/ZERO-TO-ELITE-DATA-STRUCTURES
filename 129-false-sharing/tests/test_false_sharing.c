#include "false_sharing.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
int main(void){
    FsCounterArray*packed=fs_counter_array_create(8,sizeof(uint64_t),64);assert(packed);
    assert(fs_counter_array_validate(packed));
    bool share=false;assert(fs_counters_share_line(packed,0,7,&share)&&share);
    size_t occupancy=0;assert(fs_line_occupancy(packed,3,&occupancy)&&occupancy==8U);
    assert(fs_parallel_increment(packed,4,100000U));
    for(size_t i=0;i<4U;++i){uint64_t v=0;assert(fs_counter_load(packed,i,&v)&&v==100000U);}
    fs_counter_reset(packed);
    for(size_t i=0;i<8U;++i){uint64_t v=1;assert(fs_counter_load(packed,i,&v)&&v==0U);}

    FsCounterArray*padded=fs_counter_array_create(8,64,64);assert(padded);
    assert(fs_counter_array_validate(padded));
    assert(fs_counters_share_line(padded,0,1,&share)&&!share);
    assert(fs_line_occupancy(padded,3,&occupancy)&&occupancy==1U);
    assert(fs_parallel_increment(padded,4,100000U));
    for(size_t i=0;i<4U;++i){uint64_t v=0;assert(fs_counter_load(padded,i,&v)&&v==100000U);}

    assert(fs_storage_bytes(packed)==64U);
    assert(fs_storage_bytes(padded)==512U);
    assert(!fs_counter_array_create(8,4,64));
    assert(!fs_counter_array_create(8,sizeof(uint64_t),48));
    printf("False-sharing tests passed; packed_bytes=%zu padded_bytes=%zu\n",
           fs_storage_bytes(packed),fs_storage_bytes(padded));
    fs_counter_array_free(padded);fs_counter_array_free(packed);
    return 0;
}
