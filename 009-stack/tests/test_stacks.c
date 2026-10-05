#include "array_stack.h"
#include "linked_stack.h"
#include "monotonic_stack.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void test_differential(void) {
    enum { MAX = 1024, STEPS = 10000 };

    ArrayStack *array = array_stack_create();
    LinkedStack *linked = linked_stack_create();
    assert(array != NULL && linked != NULL);

    int reference[MAX];
    size_t size = 0;
    uint32_t rng = 0xFACE1234u;

    for (int step = 0; step < STEPS; ++step) {
        const uint32_t r = next_rng(&rng);

        if ((r & 1U) == 0U && size < MAX) {
            const int value = (int)next_rng(&rng);
            assert(array_stack_push(array, value));
            assert(linked_stack_push(linked, value));
            reference[size++] = value;
        } else if (size > 0) {
            int a = 0;
            int b = 0;
            assert(array_stack_peek(array, &a));
            assert(linked_stack_peek(linked, &b));
            assert(a == reference[size - 1]);
            assert(b == reference[size - 1]);

            assert(array_stack_pop(array, &a));
            assert(linked_stack_pop(linked, &b));
            assert(a == reference[size - 1]);
            assert(b == reference[size - 1]);
            --size;
        }

        assert(array_stack_validate(array));
        assert(linked_stack_validate(linked));
        assert(array_stack_size(array) == size);
        assert(linked_stack_size(linked) == size);
        assert(array_stack_is_empty(array) == (size == 0));
        assert(linked_stack_is_empty(linked) == (size == 0));
    }

    array_stack_free(array);
    linked_stack_free(linked);
}

static void test_underflow(void) {
    ArrayStack *array = array_stack_create();
    LinkedStack *linked = linked_stack_create();
    assert(array != NULL && linked != NULL);

    int value = 0;
    assert(!array_stack_pop(array, &value));
    assert(!linked_stack_pop(linked, &value));
    assert(!array_stack_peek(array, &value));
    assert(!linked_stack_peek(linked, &value));

    array_stack_free(array);
    linked_stack_free(linked);
}

static void test_monotonic(void) {
    const int input[] = {2, 1, 5, 3, 4, 4, 9};
    const int expected[] = {5, 5, 9, 4, 9, 9, -1};
    int output[7];

    assert(next_greater_values(input, 7, -1, output));
    for (size_t i = 0; i < 7; ++i) {
        assert(output[i] == expected[i]);
    }

    const int decreasing[] = {5, 4, 3, 2, 1};
    int output2[5];
    assert(next_greater_values(decreasing, 5, -1, output2));
    for (size_t i = 0; i < 5; ++i) {
        assert(output2[i] == -1);
    }

    assert(next_greater_values(NULL, 0, -1, NULL));
}

int main(void) {
    test_differential();
    test_underflow();
    test_monotonic();

    puts("stack tests passed");
    return 0;
}
