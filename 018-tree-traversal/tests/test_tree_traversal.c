#include "tree_traversal.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static IntBinaryTree *build_complete_7(void) {
    IntBinaryTree *tree = int_binary_tree_create();
    if (tree == NULL) return NULL;

    IntBinaryNode *n[7] = {0};

    if (!int_binary_tree_set_root(tree, 1, &n[0]) ||
        !int_binary_tree_add_left(tree, n[0], 2, &n[1]) ||
        !int_binary_tree_add_right(tree, n[0], 3, &n[2]) ||
        !int_binary_tree_add_left(tree, n[1], 4, &n[3]) ||
        !int_binary_tree_add_right(tree, n[1], 5, &n[4]) ||
        !int_binary_tree_add_left(tree, n[2], 6, &n[5]) ||
        !int_binary_tree_add_right(tree, n[2], 7, &n[6])) {
        int_binary_tree_free(tree);
        return NULL;
    }

    return tree;
}

static void assert_sequence(const int *actual, const int *expected, size_t n) {
    assert(memcmp(actual, expected, n * sizeof *actual) == 0);
}

static void test_known_orders(void) {
    IntBinaryTree *tree = build_complete_7();
    assert(tree != NULL);

    const int preorder[] = {1,2,4,5,3,6,7};
    const int inorder[] = {4,2,5,1,6,3,7};
    const int postorder[] = {4,5,2,6,7,3,1};
    const int level[] = {1,2,3,4,5,6,7};

    int output[7];
    size_t written = 0;

    assert(tree_preorder_recursive(tree, output, 7, &written));
    assert(written == 7);
    assert_sequence(output, preorder, 7);

    assert(tree_preorder_iterative(tree, output, 7, &written));
    assert_sequence(output, preorder, 7);

    assert(tree_inorder_recursive(tree, output, 7, &written));
    assert_sequence(output, inorder, 7);

    assert(tree_inorder_iterative(tree, output, 7, &written));
    assert_sequence(output, inorder, 7);

    assert(tree_postorder_recursive(tree, output, 7, &written));
    assert_sequence(output, postorder, 7);

    assert(tree_postorder_iterative(tree, output, 7, &written));
    assert_sequence(output, postorder, 7);

    assert(tree_level_order(tree, output, 7, &written));
    assert_sequence(output, level, 7);

    assert(!tree_preorder_recursive(tree, output, 6, &written));

    int_binary_tree_free(tree);
}

static void test_irregular_equivalence(void) {
    IntBinaryTree *tree = int_binary_tree_create();
    assert(tree != NULL);

    IntBinaryNode *root = NULL;
    IntBinaryNode *a = NULL;
    IntBinaryNode *b = NULL;
    IntBinaryNode *c = NULL;
    IntBinaryNode *d = NULL;

    assert(int_binary_tree_set_root(tree, 10, &root));
    assert(int_binary_tree_add_left(tree, root, 20, &a));
    assert(int_binary_tree_add_right(tree, root, 30, &b));
    assert(int_binary_tree_add_right(tree, a, 40, &c));
    assert(int_binary_tree_add_left(tree, c, 50, &d));
    assert(int_binary_tree_add_left(tree, b, 60, NULL));

    const size_t n = int_binary_tree_size(tree);
    int rec[16];
    int iter[16];
    size_t rw = 0;
    size_t iw = 0;

    assert(tree_preorder_recursive(tree, rec, 16, &rw));
    assert(tree_preorder_iterative(tree, iter, 16, &iw));
    assert(rw == n && iw == n);
    assert_sequence(rec, iter, n);

    assert(tree_inorder_recursive(tree, rec, 16, &rw));
    assert(tree_inorder_iterative(tree, iter, 16, &iw));
    assert_sequence(rec, iter, n);

    assert(tree_postorder_recursive(tree, rec, 16, &rw));
    assert(tree_postorder_iterative(tree, iter, 16, &iw));
    assert_sequence(rec, iter, n);

    (void)d;
    int_binary_tree_free(tree);
}

static void test_skewed_and_empty(void) {
    IntBinaryTree *empty = int_binary_tree_create();
    assert(empty != NULL);

    size_t written = 999;
    assert(tree_preorder_recursive(empty, NULL, 0, &written));
    assert(written == 0);
    assert(tree_level_order(empty, NULL, 0, &written));

    int_binary_tree_free(empty);

    IntBinaryTree *tree = int_binary_tree_create();
    assert(tree != NULL);

    IntBinaryNode *node = NULL;
    assert(int_binary_tree_set_root(tree, 0, &node));

    for (int i = 1; i < 200; ++i) {
        IntBinaryNode *next = NULL;
        assert(int_binary_tree_add_right(tree, node, i, &next));
        node = next;
    }

    int recursive[200];
    int iterative[200];
    size_t a = 0;
    size_t b = 0;

    assert(tree_preorder_recursive(tree, recursive, 200, &a));
    assert(tree_preorder_iterative(tree, iterative, 200, &b));
    assert(a == 200 && b == 200);
    assert_sequence(recursive, iterative, 200);

    assert(tree_inorder_recursive(tree, recursive, 200, &a));
    assert(tree_inorder_iterative(tree, iterative, 200, &b));
    assert_sequence(recursive, iterative, 200);

    assert(tree_postorder_recursive(tree, recursive, 200, &a));
    assert(tree_postorder_iterative(tree, iterative, 200, &b));
    assert_sequence(recursive, iterative, 200);

    int_binary_tree_free(tree);
}

int main(void) {
    test_known_orders();
    test_irregular_equivalence();
    test_skewed_and_empty();

    puts("tree traversal tests passed");
    return 0;
}
