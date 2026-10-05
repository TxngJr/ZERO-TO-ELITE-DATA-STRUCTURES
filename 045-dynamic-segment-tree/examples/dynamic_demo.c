#include "dynamic_segment_tree.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    DynamicSegmentTree *tree=
        dynamic_segment_tree_create(
            0,
            UINT64_C(1000000000000000000)
        );
    assert(tree!=NULL);

    assert(dynamic_segment_tree_point_add(tree,3,10));
    assert(dynamic_segment_tree_point_add(
        tree,UINT64_C(999999999999999999),7
    ));
    assert(dynamic_segment_tree_point_add(
        tree,UINT64_C(500000000000000000),-2
    ));

    int64_t sum=0;

    assert(dynamic_segment_tree_range_sum(
        tree,0,
        UINT64_C(1000000000000000000),
        &sum
    ));

    printf("sum=%" PRId64 " nodes=%zu\n",
           sum,
           dynamic_segment_tree_node_count(tree));

    assert(dynamic_segment_tree_validate(tree));
    dynamic_segment_tree_free(tree);
    return 0;
}
