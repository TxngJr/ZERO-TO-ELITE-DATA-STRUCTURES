#include "int_quadtree.h"

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

    puts("n,nodes,queries,seconds");

    for(size_t s=0;s<3;++s) {
        const size_t n=sizes[s];

        IntQuadtree *tree=int_quadtree_create(
            0,1000000,0,1000000,8,24
        );
        if(tree==NULL)return 1;

        for(size_t i=0;i<n;++i) {
            rng=rng*1664525u+1013904223u;
            const int64_t x=(int64_t)(rng%1000000U);

            rng=rng*1664525u+1013904223u;
            const int64_t y=(int64_t)(rng%1000000U);

            if(!int_quadtree_insert(
                    tree,(IntQuadPoint){x,y,(uint64_t)i}
                )) {
                return 1;
            }
        }

        const size_t queries=50000;
        volatile size_t checksum=0;
        struct timespec a,b;

        timespec_get(&a,TIME_UTC);

        for(size_t q=0;q<queries;++q) {
            rng=rng*1664525u+1013904223u;
            const int64_t x=(int64_t)(rng%990000U);

            rng=rng*1664525u+1013904223u;
            const int64_t y=(int64_t)(rng%990000U);

            size_t count=0;

            if(!int_quadtree_query_count(
                    tree,x,x+10000,y,y+10000,&count
                )) {
                return 1;
            }

            checksum^=count;
        }

        timespec_get(&b,TIME_UTC);

        printf(
            "%zu,%zu,%zu,%.9f\n",
            n,
            int_quadtree_node_count(tree),
            queries,
            elapsed(a,b)
        );

        (void)checksum;
        int_quadtree_free(tree);
    }

    return 0;
}
