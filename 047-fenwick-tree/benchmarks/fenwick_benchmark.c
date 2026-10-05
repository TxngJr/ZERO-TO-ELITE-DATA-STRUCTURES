#include "int_fenwick_tree.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t sizes[]={1024,16384,262144};
    const size_t operations=200000;
    uint32_t rng=123456789u;

    puts("n,operations,seconds");

    for(size_t s=0;s<3;++s) {
        const size_t n=sizes[s];

        int64_t *values=calloc(n,sizeof *values);
        if(values==NULL)return 1;

        IntFenwickTree *tree=
            int_fenwick_tree_create(values,n);
        if(tree==NULL)return 1;

        volatile int64_t checksum=0;
        struct timespec a,b;

        timespec_get(&a,TIME_UTC);

        for(size_t op=0;op<operations;++op) {
            rng=rng*1664525u+1013904223u;

            if((rng&3U)==0U) {
                const size_t index=rng%n;
                const int64_t delta=
                    (int64_t)((rng>>8)%21U)-10;

                if(!int_fenwick_tree_point_add(
                        tree,index,delta
                    )) {
                    return 1;
                }
            } else {
                size_t left=rng%n;

                rng=rng*1664525u+1013904223u;
                size_t right=rng%n;

                if(left>right) {
                    const size_t tmp=left;
                    left=right;
                    right=tmp;
                }

                ++right;

                int64_t sum=0;

                if(!int_fenwick_tree_range_sum(
                        tree,left,right,&sum
                    )) {
                    return 1;
                }

                checksum^=sum;
            }
        }

        timespec_get(&b,TIME_UTC);

        printf("%zu,%zu,%.9f\n",
               n,operations,elapsed(a,b));

        (void)checksum;
        int_fenwick_tree_free(tree);
        free(values);
    }

    return 0;
}
