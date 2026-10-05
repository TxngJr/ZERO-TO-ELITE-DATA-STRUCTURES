#include "int_skip_list.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void test_basic_and_range(void) {
    IntSkipList *list = int_skip_list_create(16);
    assert(list != NULL);

    const int keys[] = {
        50,10,30,20,40,70,60,80
    };

    for (size_t i = 0;
         i < sizeof keys / sizeof keys[0];
         ++i) {
        assert(int_skip_list_insert(
            list,
            keys[i]
        ));
        assert(int_skip_list_validate(list));
    }

    assert(!int_skip_list_insert(list, 30));
    assert(int_skip_list_contains(list, 60));
    assert(!int_skip_list_contains(list, 65));

    int output[16];
    size_t written = 0;

    assert(int_skip_list_range(
        list,
        25,
        65,
        output,
        16,
        &written
    ));

    const int expected[] = {30,40,50,60};
    assert(written == 4);

    for (size_t i = 0; i < written; ++i) {
        assert(output[i] == expected[i]);
    }

    assert(int_skip_list_remove(list, 30));
    assert(!int_skip_list_contains(list, 30));
    assert(int_skip_list_validate(list));

    int_skip_list_free(list);
}

static void assert_reference(
    const IntSkipList *list,
    const bool *present,
    int min_key,
    int max_key
) {
    int output[2048];
    size_t written = 0;

    assert(int_skip_list_range(
        list,
        min_key,
        max_key,
        output,
        2048,
        &written
    ));

    size_t index = 0;

    for (int key = min_key;
         key <= max_key;
         ++key) {
        if (present[key - min_key]) {
            assert(index < written);
            assert(output[index++] == key);
        }
    }

    assert(index == written);
}

static void test_randomized(void) {
    enum {
        MIN=-500,
        MAX=500,
        COUNT=1001,
        STEPS=40000
    };

    IntSkipList *list = int_skip_list_create(32);
    assert(list != NULL);

    bool present[COUNT];
    memset(present, 0, sizeof present);

    size_t expected_size = 0;
    uint32_t rng = 0x5A1F0361u;

    for (int step = 0; step < STEPS; ++step) {
        const int key =
            MIN + (int)(next_rng(&rng) % COUNT);
        const size_t index = (size_t)(key - MIN);
        const unsigned op = next_rng(&rng) % 3U;

        if (op == 0U) {
            const bool inserted =
                int_skip_list_insert(list, key);

            assert(inserted == !present[index]);

            if (inserted) {
                present[index] = true;
                ++expected_size;
            }
        } else if (op == 1U) {
            const bool removed =
                int_skip_list_remove(list, key);

            assert(removed == present[index]);

            if (removed) {
                present[index] = false;
                --expected_size;
            }
        } else {
            assert(
                int_skip_list_contains(list, key) ==
                present[index]
            );
        }

        assert(int_skip_list_size(list) == expected_size);
        assert(int_skip_list_validate(list));

        if ((step % 257) == 0) {
            assert_reference(
                list,
                present,
                MIN,
                MAX
            );
        }
    }

    assert_reference(list, present, MIN, MAX);
    int_skip_list_free(list);
}

int main(void) {
    assert(int_skip_list_create(0) == NULL);

    test_basic_and_range();
    test_randomized();

    puts("Skip List tests passed");
    return 0;
}
