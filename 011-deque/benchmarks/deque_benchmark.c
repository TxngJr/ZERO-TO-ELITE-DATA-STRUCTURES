#include "int_deque.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

static double ring_work(size_t n) {
    IntDeque *d = int_deque_create();
    if (d == NULL) return -1.0;

    struct timespec a, b;
    timespec_get(&a, TIME_UTC);

    for (size_t i = 0; i < n; ++i) {
        if (!int_deque_push_back(d, (int)i)) {
            int_deque_free(d);
            return -1.0;
        }
    }

    for (size_t i = 0; i < n / 2; ++i) {
        if (!int_deque_pop_front(d, NULL)) {
            int_deque_free(d);
            return -1.0;
        }
        if (!int_deque_push_back(d, (int)i)) {
            int_deque_free(d);
            return -1.0;
        }
    }

    timespec_get(&b, TIME_UTC);
    const double result = elapsed(a, b);
    int_deque_free(d);
    return result;
}

static double shifting_reference(size_t n) {
    int *data = malloc(n * sizeof *data);
    if (data == NULL) return -1.0;

    size_t size = 0;
    struct timespec a, b;
    timespec_get(&a, TIME_UTC);

    for (size_t i = 0; i < n; ++i) data[size++] = (int)i;

    for (size_t i = 0; i < n / 2; ++i) {
        memmove(data, data + 1, (size - 1) * sizeof *data);
        --size;
        data[size++] = (int)i;
    }

    timespec_get(&b, TIME_UTC);
    const double result = elapsed(a, b);
    free(data);
    return result;
}

int main(void) {
    const size_t ns[] = {1000, 5000, 10000};

    puts("n,ring_seconds,shifting_reference_seconds");
    for (size_t i = 0; i < sizeof ns / sizeof ns[0]; ++i) {
        const double ring = ring_work(ns[i]);
        const double shift = shifting_reference(ns[i]);

        if (ring < 0.0 || shift < 0.0) return 1;

        printf("%zu,%.9f,%.9f\n", ns[i], ring, shift);
    }

    puts("The shifting reference intentionally demonstrates a different asymptotic dequeue design.");
    return 0;
}
