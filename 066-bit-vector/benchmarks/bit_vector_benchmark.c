#include "int_bit_vector.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t n=10000000;
    const size_t queries=1000000;
    uint8_t *bits=malloc(n);

    if(bits==NULL)return 1;

    uint32_t rng=123456789u;

    for(size_t i=0;i<n;++i) {
        rng=rng*1664525u+1013904223u;
        bits[i]=(uint8_t)(rng&1U);
    }

    struct timespec build_a,build_b;
    timespec_get(&build_a,TIME_UTC);

    IntBitVector *v=int_bit_vector_create(bits,n);

    timespec_get(&build_b,TIME_UTC);

    if(v==NULL)return 1;

    volatile size_t checksum=0;
    struct timespec q_a,q_b;
    timespec_get(&q_a,TIME_UTC);

    for(size_t q=0;q<queries;++q) {
        rng=rng*1664525u+1013904223u;
        const size_t end=(size_t)rng%(n+1U);
        size_t rank=0;

        if(!int_bit_vector_rank1(v,end,&rank))return 1;
        checksum^=rank;
    }

    timespec_get(&q_b,TIME_UTC);

    printf(
        "bits=%zu ones=%zu build=%.9f rank_queries=%zu rank_seconds=%.9f checksum=%zu\n",
        n,
        int_bit_vector_ones(v),
        elapsed(build_a,build_b),
        queries,
        elapsed(q_a,q_b),
        (size_t)checksum
    );

    int_bit_vector_free(v);
    free(bits);
    return 0;
}
