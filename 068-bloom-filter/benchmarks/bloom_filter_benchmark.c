#include "byte_bloom_filter.h"

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
    const size_t n=200000;
    const size_t queries=1000000;

    ByteBloomFilter *filter=byte_bloom_filter_create(
        4000000,7
    );
    if(filter==NULL)return 1;

    uint8_t key[8];
    struct timespec a,b;

    timespec_get(&a,TIME_UTC);

    for(uint64_t i=0;i<n;++i) {
        encode(i,key);
        if(!byte_bloom_filter_add(filter,key,sizeof key))return 1;
    }

    timespec_get(&b,TIME_UTC);
    const double build=elapsed(a,b);

    size_t positives=0;
    timespec_get(&a,TIME_UTC);

    for(uint64_t i=0;i<queries;++i) {
        encode(UINT64_C(1000000000)+i,key);
        bool maybe=false;

        if(!byte_bloom_filter_maybe_contains(
                filter,key,sizeof key,&maybe
            )) {
            return 1;
        }

        positives+=maybe?1U:0U;
    }

    timespec_get(&b,TIME_UTC);

    printf("n=%zu set_bits=%zu build=%.9f queries=%zu positives=%zu query_seconds=%.9f\n",
           n,
           byte_bloom_filter_set_bits(filter),
           build,queries,positives,elapsed(a,b));

    byte_bloom_filter_free(filter);
    return 0;
}
