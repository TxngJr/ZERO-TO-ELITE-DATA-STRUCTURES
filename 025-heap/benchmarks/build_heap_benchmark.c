#include "int_min_heap.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

int main(void) {
    const size_t ns[] = {1000,10000,100000,500000};

    puts("n,bottom_up_seconds,repeated_push_seconds");

    for (size_t t = 0; t < sizeof ns / sizeof ns[0]; ++t) {
        const size_t n = ns[t];
        int *values = malloc(n * sizeof *values);
        if (values == NULL) return 1;

        uint32_t x = 123456789u;

        for (size_t i = 0; i < n; ++i) {
            x = x * 1664525u + 1013904223u;
            values[i] = (int)x;
        }

        struct timespec a;
        struct timespec b;

        timespec_get(&a, TIME_UTC);
        IntMinHeap *built = int_min_heap_build(values, n);
        timespec_get(&b, TIME_UTC);
        if (built == NULL) return 1;

        const double bottom_up = elapsed(a, b);

        IntMinHeap *incremental = int_min_heap_create();
        if (incremental == NULL) return 1;

        timespec_get(&a, TIME_UTC);
        for (size_t i = 0; i < n; ++i) {
            if (!int_min_heap_push(incremental, values[i])) return 1;
        }
        timespec_get(&b, TIME_UTC);

        const double repeated = elapsed(a, b);

        printf("%zu,%.9f,%.9f\n", n, bottom_up, repeated);

        int_min_heap_free(built);
        int_min_heap_free(incremental);
        free(values);
    }

    puts("Wall-clock ratios depend on machine/compiler; asymptotic construction bounds differ.");
    return 0;
}
