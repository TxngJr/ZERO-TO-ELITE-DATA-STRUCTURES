#include "int_red_black_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t rng_next(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void assert_reference(
    const IntRedBlackTree *tree,
    const bool *present,
    int min_key,
    int max_key
) {
    int output[2048];
    size_t written = 0;

    assert(int_rbt_inorder(tree, output, 2048, &written));

    size_t index = 0;

    for (int key = min_key; key <= max_key; ++key) {
        if (present[key - min_key]) {
            assert(index < written);
            assert(output[index++] == key);
        }
    }

    assert(index == written);
}

static void test_insert_delete_sequences(void) {
    IntRedBlackTree *tree = int_rbt_create();
    assert(tree != NULL);

    const int keys[] = {
        41,38,31,12,19,8,50,60,55,1,2,3,4,5,6,7
    };

    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; ++i) {
        assert(int_rbt_insert(tree, keys[i]));
        assert(int_rbt_validate(tree));
    }

    assert(!int_rbt_insert(tree, 19));

    const int removals[] = {
        8,12,19,31,38,41,1,7,4,50,60,55,2,3,5,6
    };

    for (size_t i = 0; i < sizeof removals / sizeof removals[0]; ++i) {
        assert(int_rbt_remove(tree, removals[i]));
        assert(int_rbt_validate(tree));
    }

    assert(int_rbt_size(tree) == 0);
    assert(!int_rbt_remove(tree, 999));

    int_rbt_free(tree);
}

static void test_sorted_height(void) {
    IntRedBlackTree *tree = int_rbt_create();
    assert(tree != NULL);

    for (int key = 1; key <= 2000; ++key) {
        assert(int_rbt_insert(tree, key));
        assert(int_rbt_validate(tree));
    }

    size_t height = 0;
    assert(int_rbt_height(tree, &height));
    assert(height < 32);

    int_rbt_free(tree);
}

static void test_randomized(void) {
    enum { MIN=-500, MAX=500, COUNT=1001, STEPS=40000 };

    IntRedBlackTree *tree = int_rbt_create();
    assert(tree != NULL);

    bool present[COUNT];
    memset(present, 0, sizeof present);

    size_t expected_size = 0;
    uint32_t rng = 0x7B700001u;

    for (int step = 0; step < STEPS; ++step) {
        const int key = MIN + (int)(rng_next(&rng) % COUNT);
        const size_t idx = (size_t)(key - MIN);
        const unsigned op = rng_next(&rng) % 3U;

        if (op == 0U) {
            const bool inserted = int_rbt_insert(tree, key);
            assert(inserted == !present[idx]);

            if (inserted) {
                present[idx] = true;
                ++expected_size;
            }
        } else if (op == 1U) {
            const bool removed = int_rbt_remove(tree, key);
            assert(removed == present[idx]);

            if (removed) {
                present[idx] = false;
                --expected_size;
            }
        } else {
            assert(int_rbt_contains(tree, key) == present[idx]);
        }

        assert(int_rbt_size(tree) == expected_size);
        assert(int_rbt_validate(tree));

        if ((step % 317) == 0) {
            assert_reference(tree, present, MIN, MAX);
        }
    }

    assert_reference(tree, present, MIN, MAX);
    int_rbt_free(tree);
}

int main(void) {
    test_insert_delete_sequences();
    test_sorted_height();
    test_randomized();

    puts("Red-Black Tree tests passed");
    return 0;
}
