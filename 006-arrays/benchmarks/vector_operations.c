#include "int_vector.h"

#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

static double benchmark_push(size_t n) {
    IntVector *v = int_vector_create();
    if (v == NULL) {
        return -1.0;
    }

    struct timespec a, b;
    timespec_get(&a, TIME_UTC);
    for (size_t i = 0; i < n; ++i) {
        if (!int_vector_push(v, (int)i)) {
            int_vector_free(v);
            return -1.0;
        }
    }
    timespec_get(&b, TIME_UTC);

    const double result = elapsed(a, b);
    int_vector_free(v);
    return result;
}

static double benchmark_front_insert(size_t n) {
    IntVector *v = int_vector_create();
    if (v == NULL) {
        return -1.0;
    }

    struct timespec a, b;
    timespec_get(&a, TIME_UTC);
    for (size_t i = 0; i < n; ++i) {
        if (!int_vector_insert(v, 0, (int)i)) {
            int_vector_free(v);
            return -1.0;
        }
    }
    timespec_get(&b, TIME_UTC);

    const double result = elapsed(a, b);
    int_vector_free(v);
    return result;
}

int main(void) {
    const size_t ns[] = {1000, 2000, 4000, 8000};

    puts("n,push_seconds,front_insert_seconds");
    for (size_t i = 0; i < sizeof ns / sizeof ns[0]; ++i) {
        const double push_time = benchmark_push(ns[i]);
        const double front_time = benchmark_front_insert(ns[i]);

        if (push_time < 0.0 || front_time < 0.0) {
            fputs("allocation failure\n", stderr);
            return 1;
        }

        printf("%zu,%.9f,%.9f\n", ns[i], push_time, front_time);
    }

    return 0;
}
