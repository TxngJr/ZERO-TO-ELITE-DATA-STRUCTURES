#include "int_kd_tree.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t sizes[]={1000,10000,100000};
    uint32_t rng=123456789u;

    puts("n,queries,seconds");

    for(size_t s=0;s<3;++s) {
        const size_t n=sizes[s];

        IntKDPoint *points=malloc(n*sizeof *points);
        if(points==NULL)return 1;

        for(size_t i=0;i<n;++i) {
            rng=rng*1664525u+1013904223u;
            const int64_t x=(int64_t)(rng%1000000U);

            rng=rng*1664525u+1013904223u;
            const int64_t y=(int64_t)(rng%1000000U);

            points[i]=(IntKDPoint){
                .x=x,
                .y=y,
                .id=(uint64_t)i
            };
        }

        struct timespec build_a,build_b;
        timespec_get(&build_a,TIME_UTC);

        IntKDTree *tree=int_kd_tree_create(points,n);

        timespec_get(&build_b,TIME_UTC);

        if(tree==NULL)return 1;

        const size_t queries=50000;
        volatile uint64_t checksum=0;

        struct timespec a,b;
        timespec_get(&a,TIME_UTC);

        for(size_t q=0;q<queries;++q) {
            rng=rng*1664525u+1013904223u;
            const int64_t x=(int64_t)(rng%1000000U);

            rng=rng*1664525u+1013904223u;
            const int64_t y=(int64_t)(rng%1000000U);

            if((q&1U)==0U) {
                IntKDPoint nearest;
                long double distance=0.0L;

                if(!int_kd_tree_nearest(
                        tree,x,y,&nearest,&distance
                    )) {
                    return 1;
                }

                checksum^=nearest.id;
            } else {
                size_t count=0;

                if(!int_kd_tree_query_count(
                        tree,
                        x,x+1000,
                        y,y+1000,
                        &count
                    )) {
                    return 1;
                }

                checksum^=(uint64_t)count;
            }
        }

        timespec_get(&b,TIME_UTC);

        printf(
            "%zu,%zu,query=%.9f,build=%.9f\n",
            n,
            queries,
            elapsed(a,b),
            elapsed(build_a,build_b)
        );

        (void)checksum;
        int_kd_tree_free(tree);
        free(points);
    }

    return 0;
}
