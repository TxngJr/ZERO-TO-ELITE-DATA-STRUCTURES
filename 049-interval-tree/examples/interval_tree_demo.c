#include "int_interval_tree.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    IntIntervalTree *tree=int_interval_tree_create();
    assert(tree!=NULL);

    const IntInterval intervals[]={
        {15,20},{10,30},{17,19},{5,20},{12,15},{30,40}
    };

    for(size_t i=0;i<sizeof intervals/sizeof intervals[0];++i) {
        assert(int_interval_tree_insert(
            tree,intervals[i].low,intervals[i].high
        ));
    }

    IntInterval hit;

    assert(int_interval_tree_find_overlap(
        tree,14,16,&hit
    ));

    printf(
        "query [14,16) hit [%" PRId64 ",%" PRId64 ")\n",
        hit.low,hit.high
    );

    size_t count=0;
    assert(int_interval_tree_count_overlaps(
        tree,14,16,&count
    ));

    printf("overlap count=%zu\n",count);

    assert(int_interval_tree_validate(tree));
    int_interval_tree_free(tree);
    return 0;
}
