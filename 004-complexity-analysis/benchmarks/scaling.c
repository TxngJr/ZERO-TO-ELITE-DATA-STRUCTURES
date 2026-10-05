#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

static uint64_t linear_work(size_t n) {
    volatile uint64_t sink = 0;
    for (size_t i = 0; i < n; ++i) {
        sink += (uint64_t)(i * 2654435761u);
    }
    return sink;
}

static uint64_t quadratic_work(size_t n) {
    volatile uint64_t sink = 0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            sink += (uint64_t)((i + j) & 7U);
        }
    }
    return sink;
}

int main(void) {
    const size_t linear_ns[] = {10000, 20000, 40000, 80000};
    puts("linear:");
    for (size_t i = 0; i < sizeof linear_ns / sizeof linear_ns[0]; ++i) {
        struct timespec a, b;
        timespec_get(&a, TIME_UTC);
        const uint64_t result = linear_work(linear_ns[i]);
        timespec_get(&b, TIME_UTC);
        printf("n=%zu time=%.9f sink=%llu\n",
               linear_ns[i], elapsed(a, b), (unsigned long long)result);
    }

    const size_t quad_ns[] = {100, 200, 400, 800};
    puts("quadratic:");
    for (size_t i = 0; i < sizeof quad_ns / sizeof quad_ns[0]; ++i) {
        struct timespec a, b;
        timespec_get(&a, TIME_UTC);
        const uint64_t result = quadratic_work(quad_ns[i]);
        timespec_get(&b, TIME_UTC);
        printf("n=%zu time=%.9f sink=%llu\n",
               quad_ns[i], elapsed(a, b), (unsigned long long)result);
    }

    return 0;
}
