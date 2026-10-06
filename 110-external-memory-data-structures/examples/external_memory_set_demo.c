#include "external_memory_set.h"
#include <stdio.h>

int main(void){
    uint64_t values[]={1,3,5,7,9,11,13,15,17,19};
    ExternalMemorySet*s=ems_build(values,10U,4U);
    if(!s)return 1;
    bool found=false;
    ems_reset_io_counters(s);
    if(!ems_contains(s,13U,&found))return 2;
    printf("found=%d blocks=%zu reads=%zu\n",
           found,ems_block_count(s),ems_logical_reads(s));
    ems_free(s);return found?0:3;
}
