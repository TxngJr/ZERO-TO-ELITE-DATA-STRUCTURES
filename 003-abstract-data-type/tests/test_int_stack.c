#include "int_stack.h"

#include <assert.h>
#include <stdio.h>

static void test_new_stack_is_empty(void) {
    IntStack *stack = int_stack_create();
    assert(stack != NULL);
    assert(int_stack_is_empty(stack));
    assert(int_stack_size(stack) == 0);

    int out = 123;
    assert(!int_stack_pop(stack, &out));
    assert(!int_stack_peek(stack, &out));

    int_stack_free(stack);
}

static void test_lifo_order(void) {
    IntStack *stack = int_stack_create();
    assert(stack != NULL);

    assert(int_stack_push(stack, 10));
    assert(int_stack_push(stack, 20));
    assert(int_stack_push(stack, 30));
    assert(int_stack_size(stack) == 3);

    int out = 0;
    assert(int_stack_peek(stack, &out) && out == 30);
    assert(int_stack_size(stack) == 3);

    assert(int_stack_pop(stack, &out) && out == 30);
    assert(int_stack_pop(stack, &out) && out == 20);
    assert(int_stack_pop(stack, &out) && out == 10);
    assert(int_stack_is_empty(stack));

    int_stack_free(stack);
}

static void test_growth_and_reuse(void) {
    IntStack *stack = int_stack_create();
    assert(stack != NULL);

    for (int i = 0; i < 10000; ++i) {
        assert(int_stack_push(stack, i));
    }
    assert(int_stack_size(stack) == 10000);

    for (int i = 9999; i >= 0; --i) {
        int out = -1;
        assert(int_stack_pop(stack, &out));
        assert(out == i);
    }

    for (int i = 0; i < 32; ++i) {
        assert(int_stack_push(stack, i * 2));
    }

    assert(int_stack_size(stack) == 32);
    int_stack_free(stack);
}

static void test_invalid_arguments(void) {
    int out = 0;
    assert(!int_stack_push(NULL, 1));
    assert(!int_stack_pop(NULL, &out));
    assert(!int_stack_peek(NULL, &out));
    assert(int_stack_size(NULL) == 0);
    assert(int_stack_is_empty(NULL));

    IntStack *stack = int_stack_create();
    assert(stack != NULL);
    assert(int_stack_push(stack, 5));
    assert(!int_stack_pop(stack, NULL));
    assert(!int_stack_peek(stack, NULL));
    int_stack_free(stack);

    int_stack_free(NULL);
}

int main(void) {
    test_new_stack_is_empty();
    test_lifo_order();
    test_growth_and_reuse();
    test_invalid_arguments();

    puts("all IntStack tests passed");
    return 0;
}
