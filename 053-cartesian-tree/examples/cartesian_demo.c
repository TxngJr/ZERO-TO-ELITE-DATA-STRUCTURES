#include "int_cartesian_tree.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    const int64_t values[]={3,1,4,0,2};

    IntCartesianTree *tree=
        int_cartesian_tree_create(values,5);
    assert(tree!=NULL);

    size_t root=0;
    size_t index=0;
    int64_t value=0;

    assert(int_cartesian_tree_root(tree,&root));
    assert(root==3);

    assert(int_cartesian_tree_range_min(
        tree,1,4,&index,&value
    ));

    printf(
        "root=%zu range_min[1,4): index=%zu value=%" PRId64 "\n",
        root,index,value
    );

    assert(int_cartesian_tree_validate(tree));
    int_cartesian_tree_free(tree);
    return 0;
}
