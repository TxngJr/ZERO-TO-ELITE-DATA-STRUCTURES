#include "lazy_segment_tree.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    const int64_t values[]={1,2,3,4,5,6,7,8};

    LazySegmentTree *tree=
        lazy_segment_tree_create(values,8);
    assert(tree!=NULL);

    assert(lazy_segment_tree_range_add(
        tree,2,7,10
    ));

    int64_t sum=0;
    int64_t min=0;

    assert(lazy_segment_tree_range_sum(
        tree,0,8,&sum
    ));
    assert(lazy_segment_tree_range_min(
        tree,2,7,&min
    ));

    printf("sum=%" PRId64 " min[2,7)=%" PRId64 "\n",
           sum,min);

    assert(lazy_segment_tree_validate(tree));
    lazy_segment_tree_free(tree);
    return 0;
}
