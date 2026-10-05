#include "int_bstar_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void assert_reference(
    const IntBStarTree *tree,
    const bool *present,
    int min_key,
    int max_key
) {
    int output[2048];
    size_t written = 0;

    assert(int_bstar_tree_inorder(
        tree,
        output,
        2048,
        &written
    ));

    size_t index = 0;

    for (int key = min_key; key <= max_key; ++key) {
        if (present[key - min_key]) {
            assert(index < written);
            assert(output[index++] == key);
        }
    }

    assert(index == written);
}

static void test_orders(void) {
    const size_t orders[] = {3,6,9,12};

    for (size_t o = 0;
         o < sizeof orders / sizeof orders[0];
         ++o) {
        IntBStarTree *tree =
            int_bstar_tree_create(orders[o]);
        assert(tree != NULL);

        for (int key = 0; key < 500; ++key) {
            assert(int_bstar_tree_insert(tree, key));
            assert(int_bstar_tree_validate(tree));
        }

        assert(!int_bstar_tree_insert(tree, 100));

        for (int key = 0; key < 500; key += 37) {
            assert(int_bstar_tree_contains(tree, key));
        }

        int_bstar_tree_free(tree);
    }

    assert(int_bstar_tree_create(4) == NULL);
    assert(int_bstar_tree_create(5) == NULL);
}

static void test_rebuild_delete(void) {
    IntBStarTree *tree = int_bstar_tree_create(6);
    assert(tree != NULL);

    for (int key = 1; key <= 120; ++key) {
        assert(int_bstar_tree_insert(tree, key));
    }

    for (int key = 2; key <= 120; key += 2) {
        assert(int_bstar_tree_remove(tree, key));
        assert(int_bstar_tree_validate(tree));
    }

    for (int key = 1; key <= 120; ++key) {
        assert(int_bstar_tree_contains(tree, key) ==
               ((key & 1) != 0));
    }

    int_bstar_tree_free(tree);
}

static void test_randomized(void) {
    enum {
        MIN=-500,
        MAX=500,
        COUNT=1001,
        STEPS=12000
    };

    IntBStarTree *tree = int_bstar_tree_create(6);
    assert(tree != NULL);

    bool present[COUNT];
    memset(present, 0, sizeof present);

    size_t expected_size = 0;
    uint32_t rng = 0xB57A1200u;

    for (int step = 0; step < STEPS; ++step) {
        const int key =
            MIN + (int)(next_rng(&rng) % COUNT);
        const size_t index = (size_t)(key - MIN);
        const unsigned op = next_rng(&rng) % 4U;

        if (op <= 1U) {
            const bool inserted =
                int_bstar_tree_insert(tree, key);

            assert(inserted == !present[index]);

            if (inserted) {
                present[index] = true;
                ++expected_size;
            }
        } else if (op == 2U) {
            const bool removed =
                int_bstar_tree_remove(tree, key);

            assert(removed == present[index]);

            if (removed) {
                present[index] = false;
                --expected_size;
            }
        } else {
            assert(
                int_bstar_tree_contains(tree, key) ==
                present[index]
            );
        }

        assert(int_bstar_tree_size(tree) == expected_size);
        assert(int_bstar_tree_validate(tree));

        if ((step % 197) == 0) {
            assert_reference(
                tree,
                present,
                MIN,
                MAX
            );
        }
    }

    assert_reference(tree, present, MIN, MAX);
    int_bstar_tree_free(tree);
}

int main(void) {
    test_orders();
    test_rebuild_delete();
    test_randomized();

    puts("B* Tree tests passed");
    return 0;
}
