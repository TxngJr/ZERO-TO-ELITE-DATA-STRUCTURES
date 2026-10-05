#include "int_hash_set.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

static bool linear_contains(const int *data, size_t n, int value) {
    for (size_t i = 0; i < n; ++i) {
        if (data[i] == value) return true;
    }
    return false;
}

int main(void) {
    const size_t ns[] = {1000, 5000, 10000, 20000};

    puts("n,hash_set_seconds,linear_array_seconds");

    for (size_t t = 0; t < sizeof ns / sizeof ns[0]; ++t) {
        const size_t n = ns[t];
        IntHashSet *set = int_hash_set_create();
        int *values = malloc(n * sizeof *values);

        if (set == NULL || values == NULL) return 1;

        for (size_t i = 0; i < n; ++i) {
            values[i] = (int)(i * 3 + 1);
            if (!int_hash_set_add(set, values[i])) return 1;
        }

        volatile size_t hits = 0;
        struct timespec a, b;

        timespec_get(&a, TIME_UTC);
        for (size_t i = 0; i < n; ++i) {
            if (int_hash_set_contains(set, values[i])) ++hits;
        }
        timespec_get(&b, TIME_UTC);
        const double hash_time = elapsed(a, b);

        timespec_get(&a, TIME_UTC);
        for (size_t i = 0; i < n; ++i) {
            if (linear_contains(values, n, values[i])) ++hits;
        }
        timespec_get(&b, TIME_UTC);
        const double linear_time = elapsed(a, b);

        printf("%zu,%.9f,%.9f\n", n, hash_time, linear_time);

        free(values);
        int_hash_set_free(set);
        (void)hits;
    }

    puts("Expected hash performance depends on hash quality, load factor and workload.");
    return 0;
}
