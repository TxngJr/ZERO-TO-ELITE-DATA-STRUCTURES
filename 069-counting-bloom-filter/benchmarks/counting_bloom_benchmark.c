#include "byte_counting_bloom.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

static void encode(uint64_t value,uint8_t out[8]) {
    for(unsigned i=0;i<8;++i)out[i]=(uint8_t)(value>>(8U*i));
}

int main(void) {
    const size_t n=150000;

    ByteCountingBloom *filter=byte_counting_bloom_create(
        3000000,7
    );
    if(filter==NULL)return 1;

    uint8_t key[8];
    struct timespec a,b;

    timespec_get(&a,TIME_UTC);

    for(uint64_t i=0;i<n;++i) {
        encode(i,key);
        if(!byte_counting_bloom_add(filter,key,sizeof key))return 1;
    }

    timespec_get(&b,TIME_UTC);
    const double add_seconds=elapsed(a,b);

    timespec_get(&a,TIME_UTC);

    for(uint64_t i=0;i<n;i+=2) {
        encode(i,key);
        if(!byte_counting_bloom_remove(filter,key,sizeof key))return 1;
    }

    timespec_get(&b,TIME_UTC);

    printf("n=%zu logical=%zu nonzero=%zu add=%.9f remove_half=%.9f\n",
           n,
           byte_counting_bloom_logical_count(filter),
           byte_counting_bloom_nonzero_counters(filter),
           add_seconds,elapsed(a,b));

    byte_counting_bloom_free(filter);
    return 0;
}
