#include "int_dary_heap.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

int main(void) {
    const size_t ds[] = {2,4,8,16};
    const int n = 100000;

    puts("d,push_pop_seconds");

    for (size_t t = 0; t < sizeof ds / sizeof ds[0]; ++t) {
        IntDaryHeap *heap = int_dary_heap_create(ds[t]);
        if (heap == NULL) return 1;

        uint32_t rng = 123456789u;
        struct timespec a;
        struct timespec b;

        timespec_get(&a, TIME_UTC);

        for (int i = 0; i < n; ++i) {
            rng = rng * 1664525u + 1013904223u;
            if (!int_dary_heap_push(heap, (int)rng)) return 1;
        }

        for (int i = 0; i < n; ++i) {
            if (!int_dary_heap_pop(heap, NULL)) return 1;
        }

        timespec_get(&b, TIME_UTC);

        printf("%zu,%.9f\n", ds[t], elapsed(a, b));

        int_dary_heap_free(heap);
    }

    puts("Best d depends on workload, CPU, cache and implementation.");
    return 0;
}
