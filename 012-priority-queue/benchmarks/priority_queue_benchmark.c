#include "max_priority_queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    PriorityItem *items;
    size_t size;
    size_t capacity;
} UnsortedReference;

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

static bool ref_push(UnsortedReference *ref, PriorityItem item) {
    if (ref->size == ref->capacity) {
        size_t new_capacity = ref->capacity == 0 ? 8 : ref->capacity * 2;
        PriorityItem *p = realloc(ref->items, new_capacity * sizeof *p);
        if (p == NULL) return false;
        ref->items = p;
        ref->capacity = new_capacity;
    }

    ref->items[ref->size++] = item;
    return true;
}

static bool ref_pop_max(UnsortedReference *ref, PriorityItem *out) {
    if (ref->size == 0) return false;

    size_t best = 0;
    for (size_t i = 1; i < ref->size; ++i) {
        if (ref->items[i].priority > ref->items[best].priority) best = i;
    }

    if (out != NULL) *out = ref->items[best];
    ref->items[best] = ref->items[ref->size - 1];
    --ref->size;
    return true;
}

static double heap_work(size_t n) {
    MaxPriorityQueue *q = max_pq_create();
    if (q == NULL) return -1.0;

    struct timespec a, b;
    timespec_get(&a, TIME_UTC);

    for (size_t i = 0; i < n; ++i) {
        PriorityItem item = {
            .value=(int)i,
            .priority=(int)((i * 48271U) % 100003U)
        };
        if (!max_pq_push(q, item)) {
            max_pq_free(q);
            return -1.0;
        }
    }

    for (size_t i = 0; i < n; ++i) {
        if (!max_pq_pop(q, NULL)) {
            max_pq_free(q);
            return -1.0;
        }
    }

    timespec_get(&b, TIME_UTC);
    const double result = elapsed(a, b);
    max_pq_free(q);
    return result;
}

static double reference_work(size_t n) {
    UnsortedReference ref = {0};

    struct timespec a, b;
    timespec_get(&a, TIME_UTC);

    for (size_t i = 0; i < n; ++i) {
        PriorityItem item = {
            .value=(int)i,
            .priority=(int)((i * 48271U) % 100003U)
        };
        if (!ref_push(&ref, item)) {
            free(ref.items);
            return -1.0;
        }
    }

    for (size_t i = 0; i < n; ++i) {
        if (!ref_pop_max(&ref, NULL)) {
            free(ref.items);
            return -1.0;
        }
    }

    timespec_get(&b, TIME_UTC);
    const double result = elapsed(a, b);
    free(ref.items);
    return result;
}

int main(void) {
    const size_t ns[] = {500, 1000, 2000, 4000};

    puts("n,heap_seconds,unsorted_reference_seconds");
    for (size_t i = 0; i < sizeof ns / sizeof ns[0]; ++i) {
        const double heap = heap_work(ns[i]);
        const double ref = reference_work(ns[i]);

        if (heap < 0.0 || ref < 0.0) return 1;

        printf("%zu,%.9f,%.9f\n", ns[i], heap, ref);
    }

    puts("The benchmark compares different asymptotic operation mixes; absolute ratios are machine-dependent.");
    return 0;
}
