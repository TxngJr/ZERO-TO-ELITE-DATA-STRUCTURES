#include "int_fenwick_tree.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    const int64_t values[]={1,2,3,4,5,6,7,8};

    IntFenwickTree *tree=
        int_fenwick_tree_create(values,8);
    assert(tree!=NULL);

    int64_t sum=0;

    assert(int_fenwick_tree_prefix_sum(tree,7,&sum));
    printf("prefix[0,7)=%" PRId64 "\n",sum);

    assert(int_fenwick_tree_point_add(tree,5,10));

    assert(int_fenwick_tree_range_sum(tree,3,7,&sum));
    printf("after add: sum[3,7)=%" PRId64 "\n",sum);

    assert(int_fenwick_tree_validate(tree));
    int_fenwick_tree_free(tree);
    return 0;
}
