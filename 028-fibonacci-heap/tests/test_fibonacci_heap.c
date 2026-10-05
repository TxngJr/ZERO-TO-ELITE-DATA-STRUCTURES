#include "int_fibonacci_heap.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void test_meld_and_decrease(void) {
    IntFibonacciHeap *a = int_fib_heap_create();
    IntFibonacciHeap *b = int_fib_heap_create();
    assert(a != NULL && b != NULL);

    IntFibonacciNode *handles[8];

    for (int i = 0; i < 4; ++i) {
        handles[i] = int_fib_heap_insert(a, 100 + i * 10);
        assert(handles[i] != NULL);
    }

    for (int i = 4; i < 8; ++i) {
        handles[i] = int_fib_heap_insert(b, 100 + i * 10);
        assert(handles[i] != NULL);
    }

    assert(int_fib_heap_meld(a, b));
    assert(int_fib_heap_size(a) == 8);
    assert(int_fib_heap_size(b) == 0);
    assert(int_fib_heap_validate(a));

    assert(int_fib_heap_decrease_key(a, handles[7], -50));
    assert(int_fib_heap_validate(a));

    int min = 0;
    assert(int_fib_heap_peek_min(a, &min));
    assert(min == -50);

    assert(int_fib_heap_extract_min(a, &min));
    assert(min == -50);
    assert(int_fib_heap_validate(a));

    assert(int_fib_heap_delete_handle(a, handles[3]));
    assert(int_fib_heap_size(a) == 6);
    assert(int_fib_heap_validate(a));

    int previous = 0;
    bool first = true;

    while (int_fib_heap_extract_min(a, &min)) {
        if (!first) assert(previous <= min);
        previous = min;
        first = false;
        assert(int_fib_heap_validate(a));
    }

    int_fib_heap_free(a);
    int_fib_heap_free(b);
}

static void test_randomized_insert_extract(void) {
    enum { STEPS=25000, MAX_ITEMS=2048 };

    IntFibonacciHeap *heap = int_fib_heap_create();
    assert(heap != NULL);

    int reference[MAX_ITEMS];
    size_t count = 0;
    uint32_t rng = 0xF1B02811u;

    for (int step = 0; step < STEPS; ++step) {
        const bool insert =
            count == 0 ||
            (count < MAX_ITEMS && (next_rng(&rng) % 3U) != 0U);

        if (insert) {
            const int value = (int)next_rng(&rng);
            assert(int_fib_heap_insert(heap, value) != NULL);
            reference[count++] = value;
        } else {
            size_t min_index = 0;

            for (size_t i = 1; i < count; ++i) {
                if (reference[i] < reference[min_index]) min_index = i;
            }

            int out = 0;
            assert(int_fib_heap_extract_min(heap, &out));
            assert(out == reference[min_index]);

            reference[min_index] = reference[count - 1];
            --count;
        }

        assert(int_fib_heap_size(heap) == count);
        assert(int_fib_heap_validate(heap));

        if (count > 0) {
            int expected = reference[0];
            for (size_t i = 1; i < count; ++i) {
                if (reference[i] < expected) expected = reference[i];
            }

            int actual = 0;
            assert(int_fib_heap_peek_min(heap, &actual));
            assert(actual == expected);
        }
    }

    int_fib_heap_free(heap);
}

static void test_many_decrease_keys(void) {
    enum { N=512 };

    IntFibonacciHeap *heap = int_fib_heap_create();
    assert(heap != NULL);

    IntFibonacciNode *handles[N];

    for (int i = 0; i < N; ++i) {
        handles[i] = int_fib_heap_insert(heap, 10000 + i);
        assert(handles[i] != NULL);
    }

    int out = 0;
    assert(int_fib_heap_extract_min(heap, &out));
    assert(int_fib_heap_validate(heap));

    for (int i = 1; i < N; i += 3) {
        const int new_key = -i;
        assert(int_fib_heap_decrease_key(heap, handles[i], new_key));
        assert(int_fib_heap_validate(heap));
    }

    int previous = 0;
    bool first = true;

    while (int_fib_heap_extract_min(heap, &out)) {
        if (!first) assert(previous <= out);
        previous = out;
        first = false;
        assert(int_fib_heap_validate(heap));
    }

    int_fib_heap_free(heap);
}

int main(void) {
    test_meld_and_decrease();
    test_randomized_insert_extract();
    test_many_decrease_keys();

    puts("Fibonacci Heap tests passed");
    return 0;
}
