#include "tree_traversal.h"

#include <assert.h>
#include <stdio.h>

static void print_values(const char *name, const int *values, size_t n) {
    printf("%s:", name);
    for (size_t i = 0; i < n; ++i) printf(" %d", values[i]);
    putchar('\n');
}

int main(void) {
    IntBinaryTree *tree = int_binary_tree_create();
    assert(tree != NULL);

    IntBinaryNode *nodes[7] = {0};
    assert(int_binary_tree_set_root(tree, 1, &nodes[0]));
    assert(int_binary_tree_add_left(tree, nodes[0], 2, &nodes[1]));
    assert(int_binary_tree_add_right(tree, nodes[0], 3, &nodes[2]));
    assert(int_binary_tree_add_left(tree, nodes[1], 4, &nodes[3]));
    assert(int_binary_tree_add_right(tree, nodes[1], 5, &nodes[4]));
    assert(int_binary_tree_add_left(tree, nodes[2], 6, &nodes[5]));
    assert(int_binary_tree_add_right(tree, nodes[2], 7, &nodes[6]));

    int output[7];
    size_t written = 0;

    assert(tree_preorder_recursive(tree, output, 7, &written));
    print_values("preorder", output, written);

    assert(tree_inorder_recursive(tree, output, 7, &written));
    print_values("inorder", output, written);

    assert(tree_postorder_recursive(tree, output, 7, &written));
    print_values("postorder", output, written);

    assert(tree_level_order(tree, output, 7, &written));
    print_values("level-order", output, written);

    int_binary_tree_free(tree);
    return 0;
}
