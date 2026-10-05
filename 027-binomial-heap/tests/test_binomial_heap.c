#include "int_binomial_heap.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void test_meld_and_handles(void) {
    IntBinomialHeap *a = int_binomial_heap_create();
    IntBinomialHeap *b = int_binomial_heap_create();

    assert(a != NULL && b != NULL);

    IntBinomialNode *h10 = int_binomial_heap_insert(a, 10);
    IntBinomialNode *h30 = int_binomial_heap_insert(a, 30);
    IntBinomialNode *h20 = int_binomial_heap_insert(b, 20);
    IntBinomialNode *h40 = int_binomial_heap_insert(b, 40);

    assert(h10 && h30 && h20 && h40);
    assert(int_binomial_heap_validate(a));
    assert(int_binomial_heap_validate(b));

    assert(int_binomial_heap_meld(a, b));
    assert(int_binomial_heap_size(a) == 4);
    assert(int_binomial_heap_size(b) == 0);
    assert(int_binomial_heap_validate(a));
    assert(int_binomial_heap_validate(b));

    assert(int_binomial_node_decrease_key(h40, 1));
    assert(int_binomial_heap_validate(a));

    int min = 0;
    assert(int_binomial_heap_peek_min(a, &min));
    assert(min == 1);

    assert(int_binomial_heap_delete_handle(a, h20));
    assert(int_binomial_heap_size(a) == 3);
    assert(int_binomial_heap_validate(a));

    int previous = 0;
    bool first = true;

    while (int_binomial_heap_extract_min(a, &min)) {
        if (!first) assert(previous <= min);
        previous = min;
        first = false;
        assert(int_binomial_heap_validate(a));
    }

    int_binomial_heap_free(a);
    int_binomial_heap_free(b);

    (void)h10;
    (void)h30;
}

static void test_duplicate_keys(void) {
    IntBinomialHeap *heap = int_binomial_heap_create();
    assert(heap != NULL);

    for (int i = 0; i < 20; ++i) {
        assert(int_binomial_heap_insert(heap, 5) != NULL);
    }

    assert(int_binomial_heap_validate(heap));

    for (int i = 0; i < 20; ++i) {
        int out = 0;
        assert(int_binomial_heap_extract_min(heap, &out));
        assert(out == 5);
    }

    assert(int_binomial_heap_size(heap) == 0);
    int_binomial_heap_free(heap);
}

static void test_randomized(void) {
    enum { STEPS=25000, MAX_ITEMS=2048 };

    IntBinomialHeap *heap = int_binomial_heap_create();
    assert(heap != NULL);

    int reference[MAX_ITEMS];
    size_t count = 0;
    uint32_t rng = 0xB1002711u;

    for (int step = 0; step < STEPS; ++step) {
        const bool insert =
            count == 0 ||
            (count < MAX_ITEMS && (next_rng(&rng) % 3U) != 0U);

        if (insert) {
            const int value = (int)next_rng(&rng);
            assert(int_binomial_heap_insert(heap, value) != NULL);
            reference[count++] = value;
        } else {
            size_t min_index = 0;

            for (size_t i = 1; i < count; ++i) {
                if (reference[i] < reference[min_index]) min_index = i;
            }

            int out = 0;
            assert(int_binomial_heap_extract_min(heap, &out));
            assert(out == reference[min_index]);

            reference[min_index] = reference[count - 1];
            --count;
        }

        assert(int_binomial_heap_size(heap) == count);
        assert(int_binomial_heap_validate(heap));

        if (count > 0) {
            int min = reference[0];

            for (size_t i = 1; i < count; ++i) {
                if (reference[i] < min) min = reference[i];
            }

            int top = 0;
            assert(int_binomial_heap_peek_min(heap, &top));
            assert(top == min);
        }
    }

    int_binomial_heap_free(heap);
}

static void test_foreign_delete_rejected(void) {
    IntBinomialHeap *a = int_binomial_heap_create();
    IntBinomialHeap *b = int_binomial_heap_create();

    assert(a != NULL && b != NULL);

    IntBinomialNode *foreign = int_binomial_heap_insert(b, 7);
    assert(foreign != NULL);

    assert(!int_binomial_heap_delete_handle(a, foreign));
    assert(int_binomial_heap_validate(a));
    assert(int_binomial_heap_validate(b));

    int_binomial_heap_free(a);
    int_binomial_heap_free(b);
}

int main(void) {
    test_meld_and_handles();
    test_duplicate_keys();
    test_randomized();
    test_foreign_delete_rejected();

    puts("Binomial Heap tests passed");
    return 0;
}
