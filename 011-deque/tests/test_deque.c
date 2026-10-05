#include "int_deque.h"
#include "monotonic_queue.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void assert_matches(const IntDeque *deque, const int *ref, size_t size) {
    assert(int_deque_validate(deque));
    assert(int_deque_size(deque) == size);

    for (size_t i = 0; i < size; ++i) {
        int value = 0;
        assert(int_deque_get(deque, i, &value));
        assert(value == ref[i]);
    }

    if (size > 0) {
        int a = 0;
        int b = 0;
        assert(int_deque_front(deque, &a));
        assert(int_deque_back(deque, &b));
        assert(a == ref[0]);
        assert(b == ref[size - 1]);
    }
}

static void test_randomized(void) {
    enum { MAX = 512, STEPS = 12000 };

    IntDeque *deque = int_deque_create();
    assert(deque != NULL);

    int ref[MAX];
    size_t size = 0;
    uint32_t rng = 0xDEC0DEu;

    for (int step = 0; step < STEPS; ++step) {
        const unsigned op = next_rng(&rng) % 4U;

        if (op == 0U && size < MAX) {
            const int value = (int)next_rng(&rng);
            assert(int_deque_push_front(deque, value));
            memmove(ref + 1, ref, size * sizeof ref[0]);
            ref[0] = value;
            ++size;
        } else if (op == 1U && size < MAX) {
            const int value = (int)next_rng(&rng);
            assert(int_deque_push_back(deque, value));
            ref[size++] = value;
        } else if (op == 2U && size > 0) {
            int out = 0;
            assert(int_deque_pop_front(deque, &out));
            assert(out == ref[0]);
            memmove(ref, ref + 1, (size - 1) * sizeof ref[0]);
            --size;
        } else if (size > 0) {
            int out = 0;
            assert(int_deque_pop_back(deque, &out));
            assert(out == ref[size - 1]);
            --size;
        }

        assert_matches(deque, ref, size);
    }

    int_deque_free(deque);
}

static void test_sliding_window(void) {
    const int input[] = {1, 3, -1, -3, 5, 3, 6, 7};
    const int expected[] = {3, 3, 5, 5, 6, 7};
    int output[6];

    assert(sliding_window_maximum(input, 8, 3, output));

    for (size_t i = 0; i < 6; ++i) {
        assert(output[i] == expected[i]);
    }

    const int duplicates[] = {4, 4, 4, 4};
    int output2[3];
    assert(sliding_window_maximum(duplicates, 4, 2, output2));
    for (size_t i = 0; i < 3; ++i) assert(output2[i] == 4);

    assert(!sliding_window_maximum(input, 8, 0, output));
    assert(!sliding_window_maximum(input, 8, 9, output));
}

int main(void) {
    test_randomized();
    test_sliding_window();

    puts("deque tests passed");
    return 0;
}
