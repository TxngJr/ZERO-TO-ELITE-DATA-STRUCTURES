#include "array_stack.h"
#include "linked_stack.h"

#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

static double array_roundtrip(size_t n) {
    ArrayStack *stack = array_stack_create();
    if (stack == NULL) return -1.0;

    struct timespec a, b;
    timespec_get(&a, TIME_UTC);

    for (size_t i = 0; i < n; ++i) {
        if (!array_stack_push(stack, (int)i)) {
            array_stack_free(stack);
            return -1.0;
        }
    }
    for (size_t i = 0; i < n; ++i) {
        if (!array_stack_pop(stack, NULL)) {
            array_stack_free(stack);
            return -1.0;
        }
    }

    timespec_get(&b, TIME_UTC);
    const double result = elapsed(a, b);
    array_stack_free(stack);
    return result;
}

static double linked_roundtrip(size_t n) {
    LinkedStack *stack = linked_stack_create();
    if (stack == NULL) return -1.0;

    struct timespec a, b;
    timespec_get(&a, TIME_UTC);

    for (size_t i = 0; i < n; ++i) {
        if (!linked_stack_push(stack, (int)i)) {
            linked_stack_free(stack);
            return -1.0;
        }
    }
    for (size_t i = 0; i < n; ++i) {
        if (!linked_stack_pop(stack, NULL)) {
            linked_stack_free(stack);
            return -1.0;
        }
    }

    timespec_get(&b, TIME_UTC);
    const double result = elapsed(a, b);
    linked_stack_free(stack);
    return result;
}

int main(void) {
    const size_t ns[] = {1000, 10000, 100000};

    puts("n,array_seconds,linked_seconds");
    for (size_t i = 0; i < sizeof ns / sizeof ns[0]; ++i) {
        const double a = array_roundtrip(ns[i]);
        const double l = linked_roundtrip(ns[i]);
        if (a < 0.0 || l < 0.0) return 1;

        printf("%zu,%.9f,%.9f\n", ns[i], a, l);
    }

    puts("Interpret trends; allocator, CPU and cache state affect absolute timings.");
    return 0;
}
