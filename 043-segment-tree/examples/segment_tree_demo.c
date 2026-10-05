#include "int_segment_tree.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    const int64_t values[]={5,2,7,1,6,3};
    IntSegmentTree *tree=int_segment_tree_create(values,6);
    assert(tree!=NULL);

    int64_t sum=0;
    int64_t min=0;

    assert(int_segment_tree_range_sum(tree,1,5,&sum));
    assert(int_segment_tree_range_min(tree,1,5,&min));

    printf("[1,5): sum=%" PRId64 " min=%" PRId64 "\n",sum,min);

    assert(int_segment_tree_point_set(tree,3,10));
    assert(int_segment_tree_range_sum(tree,1,5,&sum));
    assert(int_segment_tree_range_min(tree,1,5,&min));

    printf("after a[3]=10: sum=%" PRId64 " min=%" PRId64 "\n",sum,min);

    assert(int_segment_tree_validate(tree));
    int_segment_tree_free(tree);
    return 0;
}
