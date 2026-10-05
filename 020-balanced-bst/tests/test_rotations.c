#include "rot_bst.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void assert_order_preserved(
    RotBST *tree,
    bool rotate_left,
    int key
) {
    const size_t n = rot_bst_size(tree);

    int before[64];
    int after[64];
    size_t a = 0;
    size_t b = 0;

    assert(n <= 64);
    assert(rot_bst_inorder(tree, before, 64, &a));

    const bool ok = rotate_left
        ? rot_bst_rotate_left(tree, key)
        : rot_bst_rotate_right(tree, key);

    assert(ok);
    assert(rot_bst_validate(tree));

    assert(rot_bst_inorder(tree, after, 64, &b));
    assert(a == b);
    assert(memcmp(before, after, a * sizeof before[0]) == 0);
}

static void test_single_rotations(void) {
    RotBST *tree = rot_bst_create();
    assert(tree != NULL);

    const int keys[] = {10, 5, 20, 15, 30};

    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; ++i) {
        assert(rot_bst_insert(tree, keys[i]));
    }

    assert_order_preserved(tree, true, 10);
    assert(rot_bst_node_key(rot_bst_root(tree)) == 20);

    assert_order_preserved(tree, false, 20);
    assert(rot_bst_node_key(rot_bst_root(tree)) == 10);

    assert(!rot_bst_rotate_left(tree, 5));
    assert(!rot_bst_rotate_right(tree, 30));

    rot_bst_free(tree);
}

static void test_lr_rl(void) {
    RotBST *lr = rot_bst_create();
    assert(lr != NULL);

    assert(rot_bst_insert(lr, 30));
    assert(rot_bst_insert(lr, 10));
    assert(rot_bst_insert(lr, 20));

    assert_order_preserved(lr, true, 10);
    assert_order_preserved(lr, false, 30);
    assert(rot_bst_node_key(rot_bst_root(lr)) == 20);

    RotBST *rl = rot_bst_create();
    assert(rl != NULL);

    assert(rot_bst_insert(rl, 10));
    assert(rot_bst_insert(rl, 30));
    assert(rot_bst_insert(rl, 20));

    assert_order_preserved(rl, false, 30);
    assert_order_preserved(rl, true, 10);
    assert(rot_bst_node_key(rot_bst_root(rl)) == 20);

    rot_bst_free(lr);
    rot_bst_free(rl);
}

static void test_height_change(void) {
    RotBST *tree = rot_bst_create();
    assert(tree != NULL);

    for (int i = 1; i <= 31; ++i) {
        assert(rot_bst_insert(tree, i));
    }

    size_t height = 0;
    assert(rot_bst_height(tree, &height));
    assert(height == 30);

    for (int i = 1; i <= 15; ++i) {
        assert(rot_bst_rotate_left(tree, i));
    }

    assert(rot_bst_validate(tree));
    assert(rot_bst_size(tree) == 31);

    assert(rot_bst_height(tree, &height));
    assert(height == 15);
    assert(rot_bst_node_key(rot_bst_root(tree)) == 16);

    rot_bst_free(tree);
}

int main(void) {
    test_single_rotations();
    test_lr_rl();
    test_height_change();

    puts("rotation tests passed");
    return 0;
}
