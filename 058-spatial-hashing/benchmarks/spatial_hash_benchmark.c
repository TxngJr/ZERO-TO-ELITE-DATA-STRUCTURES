#include "int_spatial_hash.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const int64_t cell_sizes[]={16,64,256};
    const size_t n=100000;
    const size_t queries=50000;

    for(size_t s=0;s<3;++s) {
        IntSpatialHash *hash=
            int_spatial_hash_create(cell_sizes[s]);

        if(hash==NULL)return 1;

        uint32_t rng=123456789u;

        for(size_t i=0;i<n;++i) {
            rng=rng*1664525u+1013904223u;
            const int64_t x=(int64_t)(rng%1000000U);

            rng=rng*1664525u+1013904223u;
            const int64_t y=(int64_t)(rng%1000000U);

            if(!int_spatial_hash_insert(
                    hash,(IntSpatialPoint){x,y,(uint64_t)i}
                )) {
                return 1;
            }
        }

        volatile size_t checksum=0;
        struct timespec a,b;

        timespec_get(&a,TIME_UTC);

        for(size_t q=0;q<queries;++q) {
            rng=rng*1664525u+1013904223u;
            const int64_t x=(int64_t)(rng%995000U);

            rng=rng*1664525u+1013904223u;
            const int64_t y=(int64_t)(rng%995000U);

            size_t count=0;

            if(!int_spatial_hash_query_count(
                    hash,x,x+5000,y,y+5000,&count
                )) {
                return 1;
            }

            checksum^=count;
        }

        timespec_get(&b,TIME_UTC);

        printf(
            "cell=%lld cells=%zu queries=%zu seconds=%.9f\n",
            (long long)cell_sizes[s],
            int_spatial_hash_cell_count(hash),
            queries,
            elapsed(a,b)
        );

        (void)checksum;
        int_spatial_hash_free(hash);
    }

    return 0;
}
