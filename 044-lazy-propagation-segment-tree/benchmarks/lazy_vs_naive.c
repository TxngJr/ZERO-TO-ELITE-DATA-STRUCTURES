#include "lazy_segment_tree.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    size_t left;
    size_t right;
    int64_t delta;
} Update;

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t n=200000;
    const size_t operations=5000;

    int64_t *initial=calloc(n,sizeof *initial);
    int64_t *naive=calloc(n,sizeof *naive);
    Update *updates=malloc(operations*sizeof *updates);

    if(initial==NULL||naive==NULL||updates==NULL)return 1;

    uint32_t rng=123456789u;

    for(size_t op=0;op<operations;++op) {
        rng=rng*1664525u+1013904223u;
        size_t l=rng%n;

        rng=rng*1664525u+1013904223u;
        size_t r=rng%n;

        if(l>r) {
            const size_t tmp=l;l=r;r=tmp;
        }

        updates[op]=(Update){
            .left=l,
            .right=r+1,
            .delta=(int64_t)((rng>>16)%21U)-10
        };
    }

    LazySegmentTree *tree=
        lazy_segment_tree_create(initial,n);
    if(tree==NULL)return 1;

    struct timespec a,b;

    timespec_get(&a,TIME_UTC);

    for(size_t op=0;op<operations;++op) {
        if(!lazy_segment_tree_range_add(
                tree,
                updates[op].left,
                updates[op].right,
                updates[op].delta
            )) {
            return 1;
        }
    }

    timespec_get(&b,TIME_UTC);
    const double lazy_seconds=elapsed(a,b);

    timespec_get(&a,TIME_UTC);

    for(size_t op=0;op<operations;++op) {
        for(size_t i=updates[op].left;
            i<updates[op].right;
            ++i) {
            naive[i]+=updates[op].delta;
        }
    }

    timespec_get(&b,TIME_UTC);
    const double naive_seconds=elapsed(a,b);

    int64_t tree_sum=0;
    int64_t naive_sum=0;

    if(!lazy_segment_tree_range_sum(tree,0,n,&tree_sum))return 1;

    for(size_t i=0;i<n;++i)naive_sum+=naive[i];

    if(tree_sum!=naive_sum)return 1;

    printf(
        "n=%zu updates=%zu lazy_seconds=%.9f naive_seconds=%.9f\n",
        n,operations,lazy_seconds,naive_seconds
    );

    free(updates);
    lazy_segment_tree_free(tree);
    free(initial);
    free(naive);
    return 0;
}
