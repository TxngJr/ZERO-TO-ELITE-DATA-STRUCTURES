#include "int_int_hash_table.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

enum { KEY_MIN = -500, KEY_MAX = 500, KEY_COUNT = 1001 };

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static size_t key_index(int key) {
    return (size_t)(key - KEY_MIN);
}

static void test_update_and_resize(void) {
    IntIntHashTable *table = int_int_hash_table_create();
    assert(table != NULL);

    assert(int_int_hash_table_put(table, 42, 1));
    assert(int_int_hash_table_put(table, 42, 2));
    assert(int_int_hash_table_size(table) == 1);

    int value = 0;
    assert(int_int_hash_table_get(table, 42, &value));
    assert(value == 2);

    const size_t initial_buckets = int_int_hash_table_bucket_count(table);

    for (int i = 0; i < 1000; ++i) {
        assert(int_int_hash_table_put(table, i + 1000, i));
        assert(int_int_hash_table_validate(table));
    }

    assert(int_int_hash_table_bucket_count(table) > initial_buckets);

    for (int i = 0; i < 1000; ++i) {
        assert(int_int_hash_table_get(table, i + 1000, &value));
        assert(value == i);
    }

    int_int_hash_table_free(table);
}

static void test_randomized_differential(void) {
    enum { STEPS = 30000 };

    IntIntHashTable *table = int_int_hash_table_create();
    assert(table != NULL);

    bool present[KEY_COUNT] = {false};
    int values[KEY_COUNT] = {0};
    size_t expected_size = 0;
    uint32_t rng = 0xC0111DEu;

    for (int step = 0; step < STEPS; ++step) {
        const int key = KEY_MIN + (int)(next_rng(&rng) % KEY_COUNT);
        const size_t idx = key_index(key);
        const unsigned op = next_rng(&rng) % 3U;

        if (op == 0U) {
            const int value = (int)next_rng(&rng);

            assert(int_int_hash_table_put(table, key, value));

            if (!present[idx]) {
                present[idx] = true;
                ++expected_size;
            }

            values[idx] = value;
        } else if (op == 1U) {
            int out = 0;
            const bool got = int_int_hash_table_get(table, key, &out);

            assert(got == present[idx]);
            if (got) assert(out == values[idx]);
        } else {
            int out = 0;
            const bool removed = int_int_hash_table_remove(table, key, &out);

            assert(removed == present[idx]);

            if (removed) {
                assert(out == values[idx]);
                present[idx] = false;
                --expected_size;
            }
        }

        assert(int_int_hash_table_size(table) == expected_size);
        assert(int_int_hash_table_validate(table));
    }

    int_int_hash_table_free(table);
}

int main(void) {
    test_update_and_resize();
    test_randomized_differential();

    puts("hash table tests passed");
    return 0;
}
