#include "int_treap.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t rng_next(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void assert_reference(
    const IntTreap *tree,
    const bool *present,
    int min_key,
    int max_key
) {
    int output[2048];
    size_t written = 0;

    assert(int_treap_inorder(tree, output, 2048, &written));

    size_t index = 0;

    for (int key = min_key; key <= max_key; ++key) {
        if (present[key - min_key]) {
            assert(index < written);
            assert(output[index++] == key);
        }
    }

    assert(index == written);
}

static void test_explicit_priorities(void) {
    IntTreap *tree = int_treap_create();
    assert(tree != NULL);

    assert(int_treap_insert_with_priority(tree, 50, 80));
    assert(int_treap_insert_with_priority(tree, 30, 40));
    assert(int_treap_insert_with_priority(tree, 70, 20));

    int root = 0;
    uint32_t priority = 0;

    assert(int_treap_root(tree, &root, &priority));
    assert(root == 70);
    assert(priority == 20);
    assert(int_treap_validate(tree));

    assert(int_treap_remove(tree, 70));
    assert(int_treap_validate(tree));
    assert(!int_treap_contains(tree, 70));

    int_treap_free(tree);
}

static void test_equal_priority_tiebreak(void) {
    IntTreap *tree = int_treap_create();
    assert(tree != NULL);

    assert(int_treap_insert_with_priority(tree, 20, 5));
    assert(int_treap_insert_with_priority(tree, 10, 5));
    assert(int_treap_insert_with_priority(tree, 30, 5));

    int root = 0;
    assert(int_treap_root(tree, &root, NULL));
    assert(root == 10);
    assert(int_treap_validate(tree));

    int_treap_free(tree);
}

static void test_sorted_random_height(void) {
    IntTreap *tree = int_treap_create();
    assert(tree != NULL);

    for (int key = 1; key <= 3000; ++key) {
        assert(int_treap_insert(tree, key));
        assert(int_treap_validate(tree));
    }

    size_t height = 0;
    assert(int_treap_height(tree, &height));
    assert(height < 100);

    int_treap_free(tree);
}

static void test_randomized(void) {
    enum { MIN=-500, MAX=500, COUNT=1001, STEPS=35000 };

    IntTreap *tree = int_treap_create();
    assert(tree != NULL);

    bool present[COUNT];
    memset(present, 0, sizeof present);

    size_t expected_size = 0;
    uint32_t rng = 0x7EA91234u;

    for (int step = 0; step < STEPS; ++step) {
        const int key = MIN + (int)(rng_next(&rng) % COUNT);
        const size_t idx = (size_t)(key - MIN);
        const unsigned op = rng_next(&rng) % 3U;

        if (op == 0U) {
            const bool inserted = int_treap_insert(tree, key);
            assert(inserted == !present[idx]);

            if (inserted) {
                present[idx] = true;
                ++expected_size;
            }
        } else if (op == 1U) {
            const bool removed = int_treap_remove(tree, key);
            assert(removed == present[idx]);

            if (removed) {
                present[idx] = false;
                --expected_size;
            }
        } else {
            assert(int_treap_contains(tree, key) == present[idx]);
        }

        assert(int_treap_size(tree) == expected_size);
        assert(int_treap_validate(tree));

        if ((step % 293) == 0) {
            assert_reference(tree, present, MIN, MAX);
        }
    }

    assert_reference(tree, present, MIN, MAX);
    int_treap_free(tree);
}

int main(void) {
    test_explicit_priorities();
    test_equal_priority_tiebreak();
    test_sorted_random_height();
    test_randomized();

    puts("Treap tests passed");
    return 0;
}
