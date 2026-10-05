#include "int_splay_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t rng_next(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void assert_reference(
    const IntSplayTree *tree,
    const bool *present,
    int min_key,
    int max_key
) {
    int output[2048];
    size_t written = 0;

    assert(int_splay_inorder(tree, output, 2048, &written));

    size_t index = 0;

    for (int key = min_key; key <= max_key; ++key) {
        if (present[key - min_key]) {
            assert(index < written);
            assert(output[index++] == key);
        }
    }

    assert(index == written);
}

static void test_splay_behavior(void) {
    IntSplayTree *tree = int_splay_create();
    assert(tree != NULL);

    const int keys[] = {10,20,30,5,15,25,35};

    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; ++i) {
        assert(int_splay_insert(tree, keys[i]));

        int root = 0;
        assert(int_splay_root_key(tree, &root));
        assert(root == keys[i]);
        assert(int_splay_validate(tree));
    }

    assert(int_splay_contains(tree, 5));

    int root = 0;
    assert(int_splay_root_key(tree, &root));
    assert(root == 5);

    assert(!int_splay_contains(tree, 999));
    assert(int_splay_validate(tree));

    assert(int_splay_remove(tree, 20));
    assert(!int_splay_contains(tree, 20));
    assert(int_splay_validate(tree));

    int_splay_free(tree);
}

static void test_randomized(void) {
    enum { MIN=-500, MAX=500, COUNT=1001, STEPS=30000 };

    IntSplayTree *tree = int_splay_create();
    assert(tree != NULL);

    bool present[COUNT];
    memset(present, 0, sizeof present);

    size_t expected_size = 0;
    uint32_t rng = 0x5A1A1234u;

    for (int step = 0; step < STEPS; ++step) {
        const int key = MIN + (int)(rng_next(&rng) % COUNT);
        const size_t idx = (size_t)(key - MIN);
        const unsigned op = rng_next(&rng) % 3U;

        if (op == 0U) {
            const bool inserted = int_splay_insert(tree, key);
            assert(inserted == !present[idx]);

            if (inserted) {
                present[idx] = true;
                ++expected_size;
            }
        } else if (op == 1U) {
            const bool removed = int_splay_remove(tree, key);
            assert(removed == present[idx]);

            if (removed) {
                present[idx] = false;
                --expected_size;
            }
        } else {
            assert(int_splay_contains(tree, key) == present[idx]);

            if (present[idx]) {
                int root = 0;
                assert(int_splay_root_key(tree, &root));
                assert(root == key);
            }
        }

        assert(int_splay_size(tree) == expected_size);
        assert(int_splay_validate(tree));

        if ((step % 251) == 0) {
            assert_reference(tree, present, MIN, MAX);
        }
    }

    assert_reference(tree, present, MIN, MAX);
    int_splay_free(tree);
}

int main(void) {
    test_splay_behavior();
    test_randomized();

    puts("Splay Tree tests passed");
    return 0;
}
