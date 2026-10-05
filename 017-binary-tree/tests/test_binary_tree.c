#include "int_binary_tree.h"

#include <assert.h>
#include <stdio.h>

static void test_structure(void) {
    IntBinaryTree *tree = int_binary_tree_create();
    assert(tree != NULL);
    assert(int_binary_tree_validate(tree));

    IntBinaryNode *nodes[7] = {0};

    assert(int_binary_tree_set_root(tree, 1, &nodes[0]));
    assert(int_binary_tree_add_left(tree, nodes[0], 2, &nodes[1]));
    assert(int_binary_tree_add_right(tree, nodes[0], 3, &nodes[2]));
    assert(int_binary_tree_add_left(tree, nodes[1], 4, &nodes[3]));
    assert(int_binary_tree_add_right(tree, nodes[1], 5, &nodes[4]));
    assert(int_binary_tree_add_left(tree, nodes[2], 6, &nodes[5]));
    assert(int_binary_tree_add_right(tree, nodes[2], 7, &nodes[6]));

    assert(int_binary_tree_size(tree) == 7);
    assert(int_binary_tree_validate(tree));

    size_t height = 99;
    assert(int_binary_tree_height(tree, &height));
    assert(height == 2);

    assert(int_binary_node_parent(nodes[4]) == nodes[1]);
    assert(int_binary_node_left(nodes[0]) == nodes[1]);
    assert(int_binary_node_right(nodes[0]) == nodes[2]);

    assert(int_binary_tree_set_value(tree, nodes[6], 70));
    assert(int_binary_node_value(nodes[6]) == 70);

    assert(int_binary_tree_remove_subtree(tree, nodes[1]));
    assert(int_binary_tree_size(tree) == 4);
    assert(int_binary_tree_validate(tree));

    assert(int_binary_tree_height(tree, &height));
    assert(height == 2);

    int_binary_tree_free(tree);
}

static void test_foreign_node_rejected(void) {
    IntBinaryTree *a = int_binary_tree_create();
    IntBinaryTree *b = int_binary_tree_create();
    assert(a != NULL && b != NULL);

    IntBinaryNode *ra = NULL;
    IntBinaryNode *rb = NULL;

    assert(int_binary_tree_set_root(a, 1, &ra));
    assert(int_binary_tree_set_root(b, 2, &rb));

    assert(!int_binary_tree_add_left(a, rb, 9, NULL));
    assert(!int_binary_tree_set_value(a, rb, 9));
    assert(!int_binary_tree_remove_subtree(a, rb));

    assert(int_binary_tree_size(a) == 1);
    assert(int_binary_tree_size(b) == 1);
    assert(int_binary_tree_validate(a));
    assert(int_binary_tree_validate(b));

    int_binary_tree_free(a);
    int_binary_tree_free(b);
}

static void test_large_complete_shape(void) {
    enum { N = 1023 };

    IntBinaryTree *tree = int_binary_tree_create();
    assert(tree != NULL);

    IntBinaryNode *nodes[N];

    assert(int_binary_tree_set_root(tree, 0, &nodes[0]));

    for (size_t i = 1; i < N; ++i) {
        const size_t parent = (i - 1) / 2;

        if ((i & 1U) != 0U) {
            assert(int_binary_tree_add_left(tree, nodes[parent], (int)i, &nodes[i]));
        } else {
            assert(int_binary_tree_add_right(tree, nodes[parent], (int)i, &nodes[i]));
        }
    }

    assert(int_binary_tree_size(tree) == N);
    assert(int_binary_tree_validate(tree));

    size_t height = 0;
    assert(int_binary_tree_height(tree, &height));
    assert(height == 9);

    int_binary_tree_free(tree);
}

int main(void) {
    test_structure();
    test_foreign_node_rejected();
    test_large_complete_shape();

    puts("binary tree tests passed");
    return 0;
}
