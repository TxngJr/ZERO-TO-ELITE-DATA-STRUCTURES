#include "int_binomial_heap.h"
#include "int_min_heap.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

int main(void) {
    const int n = 50000;

    IntBinomialHeap *a = int_binomial_heap_create();
    IntBinomialHeap *b = int_binomial_heap_create();

    IntMinHeap *binary_destination = int_min_heap_create();
    IntMinHeap *binary_source = int_min_heap_create();

    if (a == NULL || b == NULL ||
        binary_destination == NULL || binary_source == NULL) {
        return 1;
    }

    uint32_t rng = 123456789u;

    for (int i = 0; i < n; ++i) {
        rng = rng * 1664525u + 1013904223u;
        const int x = (int)rng;

        rng = rng * 1664525u + 1013904223u;
        const int y = (int)rng;

        if (int_binomial_heap_insert(a, x) == NULL ||
            int_binomial_heap_insert(b, y) == NULL ||
            !int_min_heap_push(binary_destination, x) ||
            !int_min_heap_push(binary_source, y)) {
            return 1;
        }
    }

    struct timespec start;
    struct timespec end;

    timespec_get(&start, TIME_UTC);
    if (!int_binomial_heap_meld(a, b)) return 1;
    timespec_get(&end, TIME_UTC);

    const double binomial_meld = elapsed(start, end);

    timespec_get(&start, TIME_UTC);

    int value = 0;
    while (int_min_heap_pop(binary_source, &value)) {
        if (!int_min_heap_push(binary_destination, value)) return 1;
    }

    timespec_get(&end, TIME_UTC);
    const double incremental_binary_merge = elapsed(start, end);

    printf(
        "items_per_heap=%d binomial_meld=%.9f binary_pop_push_merge=%.9f\n",
        n,
        binomial_meld,
        incremental_binary_merge
    );

    puts("Binary heaps can also bulk-rebuild if internal arrays are available; this benchmark compares public incremental merging.");

    int_binomial_heap_free(a);
    int_binomial_heap_free(b);
    int_min_heap_free(binary_destination);
    int_min_heap_free(binary_source);
    return 0;
}
