#include "int_hash_set.h"
#include "string_int_hash_map.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void test_set_randomized(void) {
    enum { MIN = -300, COUNT = 601, STEPS = 20000 };

    IntHashSet *set = int_hash_set_create();
    assert(set != NULL);

    bool present[COUNT] = {false};
    size_t expected_size = 0;
    uint32_t rng = 0x5E7A11u;

    for (int step = 0; step < STEPS; ++step) {
        const int value = MIN + (int)(next_rng(&rng) % COUNT);
        const size_t idx = (size_t)(value - MIN);

        if ((next_rng(&rng) & 1U) == 0U) {
            assert(int_hash_set_add(set, value));

            if (!present[idx]) {
                present[idx] = true;
                ++expected_size;
            }
        } else {
            const bool removed = int_hash_set_remove(set, value);
            assert(removed == present[idx]);

            if (removed) {
                present[idx] = false;
                --expected_size;
            }
        }

        assert(int_hash_set_contains(set, value) == present[idx]);
        assert(int_hash_set_size(set) == expected_size);
        assert(int_hash_set_validate(set));
    }

    int_hash_set_free(set);
}

typedef struct {
    char key[16];
    int value;
    bool present;
} RefEntry;

static void make_key(unsigned id, char *out, size_t size) {
    snprintf(out, size, "key-%03u", id);
}

static void test_string_map_randomized(void) {
    enum { KEY_COUNT = 200, STEPS = 25000 };

    StringIntHashMap *map = string_int_hash_map_create();
    assert(map != NULL);

    RefEntry ref[KEY_COUNT] = {0};
    size_t expected_size = 0;

    for (unsigned i = 0; i < KEY_COUNT; ++i) {
        make_key(i, ref[i].key, sizeof ref[i].key);
    }

    uint32_t rng = 0x0A5E1234u;

    for (int step = 0; step < STEPS; ++step) {
        const unsigned id = next_rng(&rng) % KEY_COUNT;
        const unsigned op = next_rng(&rng) % 3U;
        RefEntry *entry = &ref[id];

        if (op == 0U) {
            const int value = (int)next_rng(&rng);
            assert(string_int_hash_map_put(map, entry->key, value));

            if (!entry->present) {
                entry->present = true;
                ++expected_size;
            }

            entry->value = value;
        } else if (op == 1U) {
            int value = 0;
            const bool got = string_int_hash_map_get(map, entry->key, &value);

            assert(got == entry->present);
            if (got) assert(value == entry->value);
        } else {
            int value = 0;
            const bool removed = string_int_hash_map_remove(map, entry->key, &value);

            assert(removed == entry->present);

            if (removed) {
                assert(value == entry->value);
                entry->present = false;
                --expected_size;
            }
        }

        assert(string_int_hash_map_size(map) == expected_size);
        assert(string_int_hash_map_validate(map));
    }

    string_int_hash_map_free(map);
}

static void test_owned_keys_and_tombstones(void) {
    StringIntHashMap *map = string_int_hash_map_create();
    assert(map != NULL);

    char mutable_key[] = "original";
    assert(string_int_hash_map_put(map, mutable_key, 7));
    mutable_key[0] = 'X';

    int value = 0;
    assert(string_int_hash_map_get(map, "original", &value));
    assert(value == 7);

    for (int i = 0; i < 100; ++i) {
        char key[32];
        snprintf(key, sizeof key, "temp-%d", i);
        assert(string_int_hash_map_put(map, key, i));
    }

    for (int i = 0; i < 100; i += 2) {
        char key[32];
        snprintf(key, sizeof key, "temp-%d", i);
        assert(string_int_hash_map_remove(map, key, NULL));
    }

    for (int i = 1; i < 100; i += 2) {
        char key[32];
        snprintf(key, sizeof key, "temp-%d", i);
        assert(string_int_hash_map_get(map, key, &value));
        assert(value == i);
    }

    assert(string_int_hash_map_validate(map));
    string_int_hash_map_free(map);
}

int main(void) {
    test_set_randomized();
    test_string_map_randomized();
    test_owned_keys_and_tombstones();

    puts("hash set/map tests passed");
    return 0;
}
