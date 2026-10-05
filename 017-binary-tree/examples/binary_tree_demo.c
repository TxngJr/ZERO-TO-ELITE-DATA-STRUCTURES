#include "int_binary_tree.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntBinaryTree *tree = int_binary_tree_create();
    assert(tree != NULL);

    IntBinaryNode *n10 = NULL;
    IntBinaryNode *n20 = NULL;
    IntBinaryNode *n30 = NULL;

    assert(int_binary_tree_set_root(tree, 10, &n10));
    assert(int_binary_tree_add_left(tree, n10, 20, &n20));
    assert(int_binary_tree_add_right(tree, n10, 30, &n30));
    assert(int_binary_tree_add_left(tree, n20, 40, NULL));
    assert(int_binary_tree_add_right(tree, n20, 50, NULL));

    size_t height = 0;
    assert(int_binary_tree_height(tree, &height));

    printf("size=%zu height=%zu root=%d left=%d right=%d\n",
           int_binary_tree_size(tree),
           height,
           int_binary_node_value(n10),
           int_binary_node_value(n20),
           int_binary_node_value(n30));

    assert(int_binary_tree_validate(tree));
    int_binary_tree_free(tree);
    return 0;
}
