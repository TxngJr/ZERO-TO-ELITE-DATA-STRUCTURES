#include "int_lsm_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void test_versions_tombstones_compaction(void) {
    IntLSMTree *tree = int_lsm_tree_create(3);
    assert(tree != NULL);

    assert(int_lsm_tree_put(tree, 1, 10));
    assert(int_lsm_tree_put(tree, 2, 20));
    assert(int_lsm_tree_put(tree, 3, 30));

    assert(int_lsm_tree_put(tree, 4, 40));
    assert(int_lsm_tree_run_count(tree) == 1);

    assert(int_lsm_tree_put(tree, 2, 200));
    assert(int_lsm_tree_delete(tree, 3));

    int value = 0;

    assert(int_lsm_tree_get(tree, 2, &value));
    assert(value == 200);
    assert(!int_lsm_tree_get(tree, 3, &value));

    assert(int_lsm_tree_flush(tree));
    assert(int_lsm_tree_run_count(tree) == 2);
    assert(int_lsm_tree_validate(tree));

    int keys_before[8];
    int values_before[8];
    size_t before_count = 0;

    assert(int_lsm_tree_scan(
        tree,
        -100,
        100,
        keys_before,
        values_before,
        8,
        &before_count
    ));

    assert(int_lsm_tree_compact(tree));
    assert(int_lsm_tree_run_count(tree) == 1);
    assert(int_lsm_tree_validate(tree));

    int keys_after[8];
    int values_after[8];
    size_t after_count = 0;

    assert(int_lsm_tree_scan(
        tree,
        -100,
        100,
        keys_after,
        values_after,
        8,
        &after_count
    ));

    assert(before_count == after_count);
    assert(memcmp(
        keys_before,
        keys_after,
        before_count * sizeof keys_before[0]
    ) == 0);
    assert(memcmp(
        values_before,
        values_after,
        before_count * sizeof values_before[0]
    ) == 0);

    int_lsm_tree_free(tree);
}

static void test_scan_order(void) {
    IntLSMTree *tree = int_lsm_tree_create(4);
    assert(tree != NULL);

    const int input[] = {9,1,7,3,5,2,8,4,6};

    for (size_t i = 0;
         i < sizeof input / sizeof input[0];
         ++i) {
        assert(int_lsm_tree_put(
            tree,
            input[i],
            input[i] * 10
        ));
    }

    int keys[16];
    int values[16];
    size_t written = 0;

    assert(int_lsm_tree_scan(
        tree,
        3,
        7,
        keys,
        values,
        16,
        &written
    ));

    assert(written == 5);

    for (size_t i = 0; i < written; ++i) {
        assert(keys[i] == (int)i + 3);
        assert(values[i] == keys[i] * 10);
    }

    int_lsm_tree_free(tree);
}

static void test_randomized(void) {
    enum {
        MIN=-250,
        MAX=250,
        COUNT=501,
        STEPS=25000
    };

    IntLSMTree *tree = int_lsm_tree_create(16);
    assert(tree != NULL);

    bool present[COUNT];
    int values[COUNT];

    memset(present, 0, sizeof present);
    memset(values, 0, sizeof values);

    size_t expected_size = 0;
    uint32_t rng = 0x15A03511u;

    for (int step = 0; step < STEPS; ++step) {
        const int key =
            MIN + (int)(next_rng(&rng) % COUNT);
        const size_t index = (size_t)(key - MIN);
        const unsigned op = next_rng(&rng) % 5U;

        if (op <= 2U) {
            const int value = (int)next_rng(&rng);

            assert(int_lsm_tree_put(
                tree,
                key,
                value
            ));

            if (!present[index]) {
                present[index] = true;
                ++expected_size;
            }

            values[index] = value;
        } else if (op == 3U) {
            const bool removed =
                int_lsm_tree_delete(tree, key);

            assert(removed == present[index]);

            if (removed) {
                present[index] = false;
                --expected_size;
            }
        } else {
            int actual = 0;
            const bool found =
                int_lsm_tree_get(
                    tree,
                    key,
                    &actual
                );

            assert(found == present[index]);

            if (found) {
                assert(actual == values[index]);
            }
        }

        if ((step % 401) == 0) {
            assert(int_lsm_tree_flush(tree));
        }

        if ((step % 1601) == 0) {
            assert(int_lsm_tree_compact(tree));
        }

        assert(int_lsm_tree_size(tree) == expected_size);
        assert(int_lsm_tree_validate(tree));
    }

    for (int key = MIN; key <= MAX; ++key) {
        const size_t index = (size_t)(key - MIN);
        int actual = 0;

        const bool found =
            int_lsm_tree_get(tree, key, &actual);

        assert(found == present[index]);

        if (found) {
            assert(actual == values[index]);
        }
    }

    int_lsm_tree_free(tree);
}

int main(void) {
    test_versions_tombstones_compaction();
    test_scan_order();
    test_randomized();

    puts("LSM Tree tests passed");
    return 0;
}
