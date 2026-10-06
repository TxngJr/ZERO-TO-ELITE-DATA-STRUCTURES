#include "external_memory_set.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum{N=1000000,QUERIES=500000};
static double elapsed(struct timespec a,struct timespec b){
    return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;
}

int main(void){
    uint64_t*v=malloc((size_t)N*sizeof(*v));
    if(!v)return 1;
    for(size_t i=0;i<N;++i)v[i]=(uint64_t)i*3U+1U;
    ExternalMemorySet*s=ems_build(v,N,256U);
    if(!s)return 2;
    ems_reset_io_counters(s);

    struct timespec a,b;
    timespec_get(&a,TIME_UTC);
    size_t hits=0U;
    for(size_t q=0;q<QUERIES;++q){
        uint64_t key=(uint64_t)(q*7919U)%((uint64_t)N*3U);
        bool found=false;
        if(!ems_contains(s,key,&found))return 3;
        hits+=found?1U:0U;
    }
    timespec_get(&b,TIME_UTC);

    printf("keys=%d queries=%d seconds=%.6f logical_reads=%zu hits=%zu blocks=%zu\n",
           N,QUERIES,elapsed(a,b),ems_logical_reads(s),hits,ems_block_count(s));
    ems_free(s);free(v);return 0;
}
