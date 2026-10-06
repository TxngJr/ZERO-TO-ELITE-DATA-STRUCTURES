#include "succinct_bit_vector.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
static double elapsed(struct timespec a, struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){
    const size_t n=4000000, queries=200000;
    uint8_t *bits=malloc(n); if(!bits)return 1;
    uint64_t x=UINT64_C(0x123456789abcdef);
    for(size_t i=0;i<n;++i){x=x*UINT64_C(6364136223846793005)+1;bits[i]=(uint8_t)((x>>63)&1U);}
    struct timespec a,b,c,d; timespec_get(&a,TIME_UTC);
    SuccinctBitVector *bv=sbv_create(bits,n); timespec_get(&b,TIME_UTC); if(!bv)return 1;
    volatile size_t sink=0; timespec_get(&c,TIME_UTC);
    for(size_t i=0;i<queries;++i){size_t r=0,end=(i*104729U)%(n+1U); if(!sbv_rank1(bv,end,&r))return 1;sink+=r;}
    timespec_get(&d,TIME_UTC);
    printf("n=%zu packed_payload=%zu total_structure=%zu build=%.9f rank_queries=%zu rank_time=%.9f checksum=%zu\n",
      n,(n+7U)/8U,sbv_storage_bytes(bv),elapsed(a,b),queries,elapsed(c,d),(size_t)sink);
    sbv_free(bv);free(bits);return 0;
}
