#include "dynamic_segment_tree.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const uint64_t high=
        UINT64_C(1000000000000000000);
    const size_t updates=100000;

    DynamicSegmentTree *tree=
        dynamic_segment_tree_create(0,high);
    if(tree==NULL)return 1;

    uint64_t rng=UINT64_C(123456789123456789);
    struct timespec a,b;

    timespec_get(&a,TIME_UTC);

    for(size_t i=0;i<updates;++i) {
        rng=rng*UINT64_C(6364136223846793005)+
            UINT64_C(1442695040888963407);

        const uint64_t coordinate=rng%high;

        if(!dynamic_segment_tree_point_add(
                tree,coordinate,1
            )) {
            return 1;
        }
    }

    timespec_get(&b,TIME_UTC);

    int64_t sum=0;
    if(!dynamic_segment_tree_range_sum(
            tree,0,high,&sum
        )) {
        return 1;
    }

    printf(
        "domain=[0,10^18) updates=%zu allocated_nodes=%zu sum=%lld seconds=%.9f\n",
        updates,
        dynamic_segment_tree_node_count(tree),
        (long long)sum,
        elapsed(a,b)
    );

    puts("A dense static tree for this universe would be impossible; dynamic storage follows touched paths.");

    dynamic_segment_tree_free(tree);
    return 0;
}
