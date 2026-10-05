#include "byte_cuckoo_filter.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b){return (double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
static void encode(uint64_t x,uint8_t out[8]){for(unsigned i=0;i<8;++i)out[i]=(uint8_t)(x>>(8U*i));}

int main(void){
    const size_t n=200000;
    ByteCuckooFilter *f=byte_cuckoo_filter_create(65536);
    if(!f)return 1;
    uint8_t key[8]; struct timespec a,b;
    timespec_get(&a,TIME_UTC);
    size_t inserted=0;
    for(uint64_t i=0;i<n;++i){encode(i,key);if(!byte_cuckoo_filter_add(f,key,8))break;++inserted;}
    timespec_get(&b,TIME_UTC);
    printf("requested=%zu inserted=%zu load=%.4f seconds=%.9f\n",n,inserted,(double)inserted/(65536.0*4.0),elapsed(a,b));
    byte_cuckoo_filter_free(f);
    return 0;
}
