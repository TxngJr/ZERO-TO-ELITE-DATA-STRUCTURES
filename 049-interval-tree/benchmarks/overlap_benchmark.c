#include "int_interval_tree.h"

#include <stdint.h>
#include <stdio.h>
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
        IntIntervalTree *tree=int_interval_tree_create();
        if(tree==NULL)return 1;

        const size_t n=sizes[s];

        for(size_t i=0;i<n;++i) {
            rng=rng*1664525u+1013904223u;
            const int64_t low=(int64_t)(rng%10000000U);

            rng=rng*1664525u+1013904223u;
            const int64_t high=
                low+(int64_t)(rng%100U)+1;

            (void)int_interval_tree_insert(
                tree,low,high
            );
        }

        volatile size_t hits=0;
        struct timespec a,b;
        const size_t queries=100000;

        timespec_get(&a,TIME_UTC);

        for(size_t q=0;q<queries;++q) {
            rng=rng*1664525u+1013904223u;
            const int64_t low=(int64_t)(rng%10000000U);
            IntInterval hit;

            if(int_interval_tree_find_overlap(
                    tree,low,low+32,&hit
                )) {
                ++hits;
            }
        }

        timespec_get(&b,TIME_UTC);

        printf("%zu,%zu,%.9f\n",
               int_interval_tree_size(tree),
               queries,
               elapsed(a,b));

        (void)hits;
        int_interval_tree_free(tree);
    }

    return 0;
}
