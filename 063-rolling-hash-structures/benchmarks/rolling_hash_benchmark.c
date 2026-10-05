#include "byte_rolling_hash.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t n=1000000;
    const size_t queries=500000;
    uint8_t *text=malloc(n);

    if(text==NULL)return 1;

    uint32_t rng=123456789u;

    for(size_t i=0;i<n;++i) {
        rng=rng*1664525u+1013904223u;
        text[i]=(uint8_t)('a'+(rng%26U));
    }

    struct timespec build_a,build_b;
    timespec_get(&build_a,TIME_UTC);

    ByteRollingHash *hash=
        byte_rolling_hash_create(text,n);

    timespec_get(&build_b,TIME_UTC);

    if(hash==NULL)return 1;

    volatile uint64_t checksum=0;
    struct timespec a,b;

    timespec_get(&a,TIME_UTC);

    for(size_t q=0;q<queries;++q) {
        rng=rng*1664525u+1013904223u;
        const size_t left=rng%(n-64);

        ByteHashPair h;

        if(!byte_rolling_hash_range(
                hash,left,left+64,&h
            )) {
            return 1;
        }

        checksum^=h.first^h.second;
    }

    timespec_get(&b,TIME_UTC);

    printf(
        "n=%zu queries=%zu build=%.9f range_hashes=%.9f checksum=%" PRIu64 "\n",
        n,queries,
        elapsed(build_a,build_b),
        elapsed(a,b),
        (uint64_t)checksum
    );

    byte_rolling_hash_free(hash);
    free(text);
    return 0;
}
