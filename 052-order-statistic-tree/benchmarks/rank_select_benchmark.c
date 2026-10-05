#include "int_order_stat_tree.h"

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
        IntOrderStatTree *tree=int_order_stat_tree_create();
        if(tree==NULL)return 1;

        const size_t n=sizes[s];

        for(size_t i=0;i<n;++i) {
            if(!int_order_stat_tree_insert(
                    tree,(int64_t)(i*2U)
                )) {
                return 1;
            }
        }

        volatile size_t checksum=0;
        const size_t queries=200000;
        struct timespec a,b;

        timespec_get(&a,TIME_UTC);

        for(size_t q=0;q<queries;++q) {
            rng=rng*1664525u+1013904223u;

            if((rng&1U)==0U) {
                size_t rank=0;

                if(!int_order_stat_tree_rank(
                        tree,(int64_t)(rng%(n*2U)),&rank
                    )) {
                    return 1;
                }

                checksum^=rank;
            } else {
                int64_t key=0;

                if(!int_order_stat_tree_select(
                        tree,rng%n,&key
                    )) {
                    return 1;
                }

                checksum^=(size_t)key;
            }
        }

        timespec_get(&b,TIME_UTC);

        printf("%zu,%zu,%.9f\n",
               n,queries,elapsed(a,b));

        (void)checksum;
        int_order_stat_tree_free(tree);
    }

    return 0;
}
