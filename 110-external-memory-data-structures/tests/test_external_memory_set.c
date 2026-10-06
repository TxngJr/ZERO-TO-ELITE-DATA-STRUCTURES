#include "external_memory_set.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum{N=100000,QUERIES=20000,BLOCK=128};
static uint64_t state=UINT64_C(0xdecafbad1234567);
static uint64_t rng(void){state^=state<<13;state^=state>>7;state^=state<<17;return state;}

int main(void){
    uint64_t*values=malloc((size_t)N*sizeof(*values));
    assert(values);
    for(size_t i=0;i<N;++i)values[i]=(uint64_t)i*2U+1U;

    ExternalMemorySet*s=ems_build(values,N,BLOCK);
    assert(s&&ems_validate(s));
    size_t expected_blocks=(N+BLOCK-1U)/BLOCK;
    assert(ems_block_count(s)==expected_blocks);
    assert(ems_logical_writes(s)==expected_blocks);

    ems_reset_io_counters(s);
    for(size_t q=0;q<QUERIES;++q){
        uint64_t key=rng()%((uint64_t)N*2U+100U);
        size_t before=ems_logical_reads(s);
        bool found=false;
        assert(ems_contains(s,key,&found));
        bool expected=key<((uint64_t)N*2U)&&((key&1U)!=0U);
        assert(found==expected);
        assert(ems_logical_reads(s)-before<=1U);
    }

    uint64_t out[256];
    size_t out_count=0U;
    ems_reset_io_counters(s);
    assert(ems_range_collect(s,1999U,2399U,out,256U,&out_count));
    assert(out_count==201U);
    for(size_t i=0;i<out_count;++i)assert(out[i]==1999U+(uint64_t)i*2U);
    assert(ems_logical_reads(s)>=2U&&ems_logical_reads(s)<=4U);

    puts("External-memory set tests passed");
    ems_free(s);free(values);
    return 0;
}
