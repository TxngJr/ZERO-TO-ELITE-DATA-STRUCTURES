#include "recursion_iteration.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

int main(void) {
    enum { REPEATS = 500000 };
    volatile unsigned input = 20U;
    volatile uint64_t sink = 0;
    struct timespec a, b;

    timespec_get(&a, TIME_UTC);
    for (int i = 0; i < REPEATS; ++i) {
        sink ^= factorial_recursive(input);
    }
    timespec_get(&b, TIME_UTC);
    const double recursive_time = elapsed(a, b);

    timespec_get(&a, TIME_UTC);
    for (int i = 0; i < REPEATS; ++i) {
        sink ^= factorial_iterative(input);
    }
    timespec_get(&b, TIME_UTC);
    const double iterative_time = elapsed(a, b);

    printf("recursive=%.6f s iterative=%.6f s sink=%llu\n",
           recursive_time,
           iterative_time,
           (unsigned long long)sink);
    puts("This microbenchmark is descriptive, not a universal performance law.");
    return 0;
}
