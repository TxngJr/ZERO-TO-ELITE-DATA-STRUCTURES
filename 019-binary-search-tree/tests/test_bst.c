#include "int_bst.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t rng_next(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void assert_inorder_reference(
    const IntBST *tree,
    const bool *present,
    int min_key,
    int max_key
) {
    int output[2048];
    size_t written = 0;
    assert(int_bst_inorder(tree, output, 2048, &written));

    size_t index = 0;
    for (int key = min_key; key <= max_key; ++key) {
        if (present[key - min_key]) {
            assert(index < written);
            assert(output[index++] == key);
        }
    }
    assert(index == written);
}

static void test_known_delete_cases(void) {
    const int keys[] = {8,3,10,1,6,14,4,7,13};
    IntBST *tree = int_bst_create();
    assert(tree != NULL);

    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; ++i) {
        assert(int_bst_insert(tree, keys[i], NULL));
    }

    assert(!int_bst_insert(tree, 6, NULL));
    assert(int_bst_validate(tree));

    assert(int_bst_remove(tree, 1));
    assert(int_bst_validate(tree));

    assert(int_bst_remove(tree, 14));
    assert(int_bst_validate(tree));

    assert(int_bst_remove(tree, 3));
    assert(int_bst_validate(tree));

    assert(int_bst_remove(tree, 8));
    assert(int_bst_validate(tree));

    assert(!int_bst_remove(tree, 999));

    int_bst_free(tree);
}

static void test_successor_predecessor(void) {
    IntBST *tree = int_bst_create();
    assert(tree != NULL);

    const int keys[] = {20,10,30,5,15,25,35,13,17};
    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; ++i) {
        assert(int_bst_insert(tree, keys[i], NULL));
    }

    IntBSTNode *n15 = int_bst_find(tree, 15);
    assert(n15 != NULL);
    assert(int_bst_node_key(int_bst_predecessor(n15)) == 13);
    assert(int_bst_node_key(int_bst_successor(n15)) == 17);

    assert(int_bst_predecessor(int_bst_minimum(tree)) == NULL);
    assert(int_bst_successor(int_bst_maximum(tree)) == NULL);

    int_bst_free(tree);
}

static void test_randomized(void) {
    enum { MIN=-500, MAX=500, COUNT=1001, STEPS=30000 };

    IntBST *tree = int_bst_create();
    assert(tree != NULL);

    bool present[COUNT];
    memset(present, 0, sizeof present);
    size_t expected_size = 0;
    uint32_t rng = 0xB5712345u;

    for (int step = 0; step < STEPS; ++step) {
        const int key = MIN + (int)(rng_next(&rng) % COUNT);
        const size_t idx = (size_t)(key - MIN);

        if ((rng_next(&rng) & 1U) == 0U) {
            const bool inserted = int_bst_insert(tree, key, NULL);
            assert(inserted == !present[idx]);

            if (inserted) {
                present[idx] = true;
                ++expected_size;
            }
        } else {
            const bool removed = int_bst_remove(tree, key);
            assert(removed == present[idx]);

            if (removed) {
                present[idx] = false;
                --expected_size;
            }
        }

        assert(int_bst_size(tree) == expected_size);
        assert(int_bst_validate(tree));
        assert(int_bst_contains(tree, key) == present[idx]);

        if ((step % 257) == 0) {
            assert_inorder_reference(tree, present, MIN, MAX);
        }
    }

    assert_inorder_reference(tree, present, MIN, MAX);
    int_bst_free(tree);
}

static void test_sorted_shape(void) {
    IntBST *tree = int_bst_create();
    assert(tree != NULL);

    for (int i = 0; i < 100; ++i) assert(int_bst_insert(tree, i, NULL));

    size_t height = 0;
    assert(int_bst_height(tree, &height));
    assert(height == 99);

    int_bst_free(tree);
}

int main(void) {
    test_known_delete_cases();
    test_successor_predecessor();
    test_randomized();
    test_sorted_shape();

    puts("BST tests passed");
    return 0;
}
