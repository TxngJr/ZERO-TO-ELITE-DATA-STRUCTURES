#include "heap_sort.h"
#include "int_min_heap.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static int cmp_int(const void *a, const void *b) {
    const int x = *(const int *)a;
    const int y = *(const int *)b;
    return (x > y) - (x < y);
}

static void test_build_and_sort(void) {
    const int input[] = {9,4,7,1,3,6,2};

    IntMinHeap *heap = int_min_heap_build(input, 7);
    assert(heap != NULL);
    assert(int_min_heap_validate(heap));

    const int expected[] = {1,2,3,4,6,7,9};

    for (size_t i = 0; i < 7; ++i) {
        int out = 0;
        assert(int_min_heap_pop(heap, &out));
        assert(out == expected[i]);
        assert(int_min_heap_validate(heap));
    }

    int_min_heap_free(heap);

    int values[] = {9,-1,7,7,3,0,100,-50};
    int reference[8];
    memcpy(reference, values, sizeof values);

    qsort(reference, 8, sizeof reference[0], cmp_int);
    int_heap_sort_ascending(values, 8);

    assert(memcmp(values, reference, sizeof values) == 0);
}

static void test_randomized(void) {
    enum { STEPS=30000, MAX_ITEMS=4096 };

    IntMinHeap *heap = int_min_heap_create();
    assert(heap != NULL);

    int reference[MAX_ITEMS];
    size_t count = 0;
    uint32_t rng = 0x02502525u;

    for (int step = 0; step < STEPS; ++step) {
        const bool do_push =
            count == 0 ||
            (count < MAX_ITEMS && (next_rng(&rng) % 3U) != 0U);

        if (do_push) {
            const int value = (int)next_rng(&rng);
            assert(int_min_heap_push(heap, value));
            reference[count++] = value;
        } else {
            size_t min_index = 0;

            for (size_t i = 1; i < count; ++i) {
                if (reference[i] < reference[min_index]) min_index = i;
            }

            int out = 0;
            assert(int_min_heap_pop(heap, &out));
            assert(out == reference[min_index]);

            reference[min_index] = reference[count - 1];
            --count;
        }

        assert(int_min_heap_size(heap) == count);
        assert(int_min_heap_validate(heap));

        if (count > 0) {
            int min = reference[0];

            for (size_t i = 1; i < count; ++i) {
                if (reference[i] < min) min = reference[i];
            }

            int top = 0;
            assert(int_min_heap_peek(heap, &top));
            assert(top == min);
        }
    }

    int_min_heap_free(heap);
}

int main(void) {
    test_build_and_sort();
    test_randomized();

    puts("Heap tests passed");
    return 0;
}
