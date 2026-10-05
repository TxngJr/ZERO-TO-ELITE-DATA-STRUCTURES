#include "int_avl.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t rng_next(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void assert_reference(
    const IntAVL *tree,
    const bool *present,
    int min_key,
    int max_key
) {
    int output[2048];
    size_t written = 0;

    assert(int_avl_inorder(tree, output, 2048, &written));

    size_t index = 0;

    for (int key = min_key; key <= max_key; ++key) {
        if (present[key - min_key]) {
            assert(index < written);
            assert(output[index++] == key);
        }
    }

    assert(index == written);
}

static void test_rotation_patterns(void) {
    const int cases[4][3] = {
        {30,20,10},
        {10,20,30},
        {30,10,20},
        {10,30,20}
    };

    for (size_t c = 0; c < 4; ++c) {
        IntAVL *tree = int_avl_create();
        assert(tree != NULL);

        for (size_t i = 0; i < 3; ++i) {
            assert(int_avl_insert(tree, cases[c][i]));
            assert(int_avl_validate(tree));
        }

        int output[3];
        size_t written = 0;
        assert(int_avl_inorder(tree, output, 3, &written));

        assert(written == 3);
        assert(output[0] == 10);
        assert(output[1] == 20);
        assert(output[2] == 30);

        size_t height = 99;
        assert(int_avl_height(tree, &height));
        assert(height == 1);

        int_avl_free(tree);
    }
}

static void test_sorted_insert_height(void) {
    IntAVL *tree = int_avl_create();
    assert(tree != NULL);

    for (int key = 1; key <= 1000; ++key) {
        assert(int_avl_insert(tree, key));
        assert(int_avl_validate(tree));
    }

    size_t height = 0;
    assert(int_avl_height(tree, &height));
    assert(height < 20);

    assert(!int_avl_insert(tree, 500));
    assert(int_avl_size(tree) == 1000);

    int_avl_free(tree);
}

static void test_deletion(void) {
    IntAVL *tree = int_avl_create();
    assert(tree != NULL);

    for (int key = 1; key <= 200; ++key) {
        assert(int_avl_insert(tree, key));
    }

    for (int key = 2; key <= 200; key += 2) {
        assert(int_avl_remove(tree, key));
        assert(int_avl_validate(tree));
    }

    for (int key = 1; key <= 200; ++key) {
        assert(int_avl_contains(tree, key) == ((key & 1) != 0));
    }

    assert(!int_avl_remove(tree, 5000));

    int_avl_free(tree);
}

static void test_randomized(void) {
    enum { MIN=-500, MAX=500, COUNT=1001, STEPS=40000 };

    IntAVL *tree = int_avl_create();
    assert(tree != NULL);

    bool present[COUNT];
    memset(present, 0, sizeof present);

    size_t expected_size = 0;
    uint32_t rng = 0xA71A71u;

    for (int step = 0; step < STEPS; ++step) {
        const int key = MIN + (int)(rng_next(&rng) % COUNT);
        const size_t idx = (size_t)(key - MIN);

        if ((rng_next(&rng) & 1U) == 0U) {
            const bool inserted = int_avl_insert(tree, key);
            assert(inserted == !present[idx]);

            if (inserted) {
                present[idx] = true;
                ++expected_size;
            }
        } else {
            const bool removed = int_avl_remove(tree, key);
            assert(removed == present[idx]);

            if (removed) {
                present[idx] = false;
                --expected_size;
            }
        }

        assert(int_avl_size(tree) == expected_size);
        assert(int_avl_validate(tree));
        assert(int_avl_contains(tree, key) == present[idx]);

        if ((step % 313) == 0) {
            assert_reference(tree, present, MIN, MAX);
        }
    }

    assert_reference(tree, present, MIN, MAX);
    int_avl_free(tree);
}

int main(void) {
    test_rotation_patterns();
    test_sorted_insert_height();
    test_deletion();
    test_randomized();

    puts("AVL tests passed");
    return 0;
}
