#include "int_binomial_heap.h"
#include "int_fibonacci_heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

int main(void) {
    const int n = 30000;

    IntFibonacciHeap *fib = int_fib_heap_create();
    IntBinomialHeap *bin = int_binomial_heap_create();

    IntFibonacciNode **fib_handles = malloc((size_t)n * sizeof *fib_handles);
    IntBinomialNode **bin_handles = malloc((size_t)n * sizeof *bin_handles);

    if (fib == NULL || bin == NULL ||
        fib_handles == NULL || bin_handles == NULL) {
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        const int key = 1000000 + i;

        fib_handles[i] = int_fib_heap_insert(fib, key);
        bin_handles[i] = int_binomial_heap_insert(bin, key);

        if (fib_handles[i] == NULL || bin_handles[i] == NULL) return 1;
    }

    int tmp = 0;
    if (!int_fib_heap_extract_min(fib, &tmp)) return 1;
    if (!int_binomial_heap_extract_min(bin, &tmp)) return 1;

    struct timespec a;
    struct timespec b;

    timespec_get(&a, TIME_UTC);
    for (int i = 1; i < n; i += 2) {
        if (!int_fib_heap_decrease_key(fib, fib_handles[i], -i)) return 1;
    }
    timespec_get(&b, TIME_UTC);
    const double fib_time = elapsed(a, b);

    timespec_get(&a, TIME_UTC);
    for (int i = 1; i < n; i += 2) {
        if (!int_binomial_node_decrease_key(bin_handles[i], -i)) return 1;
    }
    timespec_get(&b, TIME_UTC);
    const double bin_time = elapsed(a, b);

    printf("n=%d fibonacci_decrease=%.9f binomial_decrease=%.9f\n",
           n,
           fib_time,
           bin_time);

    puts("Timing is implementation/machine specific; the theoretical distinction is amortized.");

    free(fib_handles);
    free(bin_handles);
    int_fib_heap_free(fib);
    int_binomial_heap_free(bin);
    return 0;
}
