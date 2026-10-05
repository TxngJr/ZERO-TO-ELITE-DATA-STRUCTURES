#include "int_dary_heap.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void test_same_pop_sequence(size_t d) {
    const int values[] = {9,4,7,1,3,6,2,8,5,0,-2,11};

    IntDaryHeap *heap = int_dary_heap_build(d, values, 12);
    assert(heap != NULL);
    assert(int_dary_heap_validate(heap));

    int previous = 0;

    for (size_t i = 0; i < 12; ++i) {
        int out = 0;
        assert(int_dary_heap_pop(heap, &out));

        if (i > 0) assert(previous <= out);
        previous = out;

        assert(int_dary_heap_validate(heap));
    }

    int_dary_heap_free(heap);
}

static void test_randomized(size_t d) {
    enum { STEPS=20000, MAX_ITEMS=2048 };

    IntDaryHeap *heap = int_dary_heap_create(d);
    assert(heap != NULL);

    int reference[MAX_ITEMS];
    size_t count = 0;
    uint32_t rng = (uint32_t)(0xD0000000u + d);

    for (int step = 0; step < STEPS; ++step) {
        const bool push =
            count == 0 ||
            (count < MAX_ITEMS && (next_rng(&rng) % 3U) != 0U);

        if (push) {
            const int value = (int)next_rng(&rng);
            assert(int_dary_heap_push(heap, value));
            reference[count++] = value;
        } else {
            size_t min_index = 0;

            for (size_t i = 1; i < count; ++i) {
                if (reference[i] < reference[min_index]) min_index = i;
            }

            int out = 0;
            assert(int_dary_heap_pop(heap, &out));
            assert(out == reference[min_index]);

            reference[min_index] = reference[count - 1];
            --count;
        }

        assert(int_dary_heap_size(heap) == count);
        assert(int_dary_heap_validate(heap));
    }

    int_dary_heap_free(heap);
}

int main(void) {
    assert(int_dary_heap_create(0) == NULL);
    assert(int_dary_heap_create(1) == NULL);

    const size_t ds[] = {2,3,4,8,16};

    for (size_t i = 0; i < sizeof ds / sizeof ds[0]; ++i) {
        test_same_pop_sequence(ds[i]);
        test_randomized(ds[i]);
    }

    puts("D-ary Heap tests passed");
    return 0;
}
