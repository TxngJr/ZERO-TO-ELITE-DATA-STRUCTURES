#include "int_r_tree.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t sizes[]={1000,10000,50000};
    uint32_t rng=123456789u;

    puts("n,nodes,queries,seconds");

    for(size_t s=0;s<3;++s) {
        const size_t n=sizes[s];

        IntRTree *tree=int_r_tree_create();
        if(tree==NULL)return 1;

        for(size_t i=0;i<n;++i) {
            rng=rng*1664525u+1013904223u;
            const int64_t xl=(int64_t)(rng%1000000U);

            rng=rng*1664525u+1013904223u;
            const int64_t yl=(int64_t)(rng%1000000U);

            rng=rng*1664525u+1013904223u;
            const int64_t w=(int64_t)(rng%1000U)+1;

            rng=rng*1664525u+1013904223u;
            const int64_t h=(int64_t)(rng%1000U)+1;

            if(!int_r_tree_insert(
                    tree,
                    (IntRRect){
                        xl,xl+w,yl,yl+h,(uint64_t)i
                    }
                )) {
                return 1;
            }
        }

        const size_t queries=30000;
        volatile size_t checksum=0;
        struct timespec a,b;

        timespec_get(&a,TIME_UTC);

        for(size_t q=0;q<queries;++q) {
            rng=rng*1664525u+1013904223u;
            const int64_t xl=(int64_t)(rng%990000U);

            rng=rng*1664525u+1013904223u;
            const int64_t yl=(int64_t)(rng%990000U);

            size_t count=0;

            if(!int_r_tree_query_count(
                    tree,
                    xl,xl+10000,
                    yl,yl+10000,
                    &count
                )) {
                return 1;
            }

            checksum^=count;
        }

        timespec_get(&b,TIME_UTC);

        printf(
            "%zu,%zu,%zu,%.9f\n",
            n,
            int_r_tree_node_count(tree),
            queries,
            elapsed(a,b)
        );

        (void)checksum;
        int_r_tree_free(tree);
    }

    return 0;
}
