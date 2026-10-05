#include "circular_queue.h"
#include "linked_queue.h"

#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

static double circular_roundtrip(size_t n) {
    CircularQueue *q = circular_queue_create();
    if (q == NULL) return -1.0;

    struct timespec a, b;
    timespec_get(&a, TIME_UTC);

    for (size_t i = 0; i < n; ++i) {
        if (!circular_queue_enqueue(q, (int)i)) {
            circular_queue_free(q);
            return -1.0;
        }
    }

    for (size_t i = 0; i < n; ++i) {
        if (!circular_queue_dequeue(q, NULL)) {
            circular_queue_free(q);
            return -1.0;
        }
    }

    timespec_get(&b, TIME_UTC);
    const double result = elapsed(a, b);
    circular_queue_free(q);
    return result;
}

static double linked_roundtrip(size_t n) {
    LinkedQueue *q = linked_queue_create();
    if (q == NULL) return -1.0;

    struct timespec a, b;
    timespec_get(&a, TIME_UTC);

    for (size_t i = 0; i < n; ++i) {
        if (!linked_queue_enqueue(q, (int)i)) {
            linked_queue_free(q);
            return -1.0;
        }
    }

    for (size_t i = 0; i < n; ++i) {
        if (!linked_queue_dequeue(q, NULL)) {
            linked_queue_free(q);
            return -1.0;
        }
    }

    timespec_get(&b, TIME_UTC);
    const double result = elapsed(a, b);
    linked_queue_free(q);
    return result;
}

int main(void) {
    const size_t ns[] = {1000, 10000, 100000};

    puts("n,circular_seconds,linked_seconds");
    for (size_t i = 0; i < sizeof ns / sizeof ns[0]; ++i) {
        const double c = circular_roundtrip(ns[i]);
        const double l = linked_roundtrip(ns[i]);

        if (c < 0.0 || l < 0.0) return 1;

        printf("%zu,%.9f,%.9f\n", ns[i], c, l);
    }

    puts("Interpret trends; allocator and cache behavior affect absolute timings.");
    return 0;
}
