#include "int_segment_tree.h"

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
    uint32_t rng=123456789u;

    puts("n,operations,seconds");

    for(size_t s=0;s<3;++s) {
        const size_t n=sizes[s];
        int64_t *values=malloc(n*sizeof *values);
        if(values==NULL)return 1;

        for(size_t i=0;i<n;++i)values[i]=(int64_t)(i%97);

        IntSegmentTree *tree=int_segment_tree_create(values,n);
        if(tree==NULL)return 1;

        struct timespec a,b;
        volatile int64_t checksum=0;
        const size_t operations=100000;

        timespec_get(&a,TIME_UTC);

        for(size_t i=0;i<operations;++i) {
            rng=rng*1664525u+1013904223u;

            if((rng&3U)==0U) {
                const size_t index=rng%n;
                const int64_t value=(int64_t)(rng%1000U);
                if(!int_segment_tree_point_set(
                        tree,index,value
                    )) return 1;
            } else {
                size_t left=rng%n;
                rng=rng*1664525u+1013904223u;
                size_t right=rng%n;

                if(left>right) {
                    const size_t tmp=left;
                    left=right;
                    right=tmp;
                }

                if(right==left) {
                    right=left+1;
                } else {
                    ++right;
                }

                if(right>n)right=n;

                int64_t sum=0;
                if(!int_segment_tree_range_sum(
                        tree,left,right,&sum
                    )) return 1;
                checksum^=sum;
            }
        }

        timespec_get(&b,TIME_UTC);

        printf("%zu,%zu,%.9f\n",
               n,operations,elapsed(a,b));

        (void)checksum;
        int_segment_tree_free(tree);
        free(values);
    }

    return 0;
}
