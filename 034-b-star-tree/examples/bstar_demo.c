#include "int_bstar_tree.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntBStarTree *tree = int_bstar_tree_create(6);
    assert(tree != NULL);

    for (int key = 1; key <= 100; ++key) {
        assert(int_bstar_tree_insert(tree, key));
        assert(int_bstar_tree_validate(tree));
    }

    size_t height = 0;
    assert(int_bstar_tree_height(tree, &height));

    printf(
        "size=%zu nodes=%zu height=%zu utilization=%.3f\n",
        int_bstar_tree_size(tree),
        int_bstar_tree_node_count(tree),
        height,
        int_bstar_tree_utilization(tree)
    );

    assert(int_bstar_tree_remove(tree, 50));
    assert(!int_bstar_tree_contains(tree, 50));
    assert(int_bstar_tree_validate(tree));

    int_bstar_tree_free(tree);
    return 0;
}
