#include "int_stack.h"

#include <stdint.h>
#include <stdlib.h>

struct IntStack {
    int *data;
    size_t size;
    size_t capacity;
};

static bool ensure_capacity(IntStack *stack, size_t required) {
    if (required <= stack->capacity) {
        return true;
    }

    size_t new_capacity = stack->capacity == 0 ? 4 : stack->capacity;

    while (new_capacity < required) {
        if (new_capacity > SIZE_MAX / 2) {
            return false;
        }
        new_capacity *= 2;
    }

    if (new_capacity > SIZE_MAX / sizeof *stack->data) {
        return false;
    }

    int *new_data = realloc(stack->data, new_capacity * sizeof *stack->data);
    if (new_data == NULL) {
        return false;
    }

    stack->data = new_data;
    stack->capacity = new_capacity;
    return true;
}

IntStack *int_stack_create(void) {
    return calloc(1, sizeof(IntStack));
}

void int_stack_free(IntStack *stack) {
    if (stack == NULL) {
        return;
    }

    free(stack->data);
    stack->data = NULL;
    stack->size = 0;
    stack->capacity = 0;
    free(stack);
}

bool int_stack_push(IntStack *stack, int value) {
    if (stack == NULL) {
        return false;
    }

    if (stack->size == SIZE_MAX) {
        return false;
    }

    if (!ensure_capacity(stack, stack->size + 1)) {
        return false;
    }

    stack->data[stack->size] = value;
    ++stack->size;
    return true;
}

bool int_stack_pop(IntStack *stack, int *out) {
    if (stack == NULL || out == NULL || stack->size == 0) {
        return false;
    }

    --stack->size;
    *out = stack->data[stack->size];
    return true;
}

bool int_stack_peek(const IntStack *stack, int *out) {
    if (stack == NULL || out == NULL || stack->size == 0) {
        return false;
    }

    *out = stack->data[stack->size - 1];
    return true;
}

size_t int_stack_size(const IntStack *stack) {
    return stack == NULL ? 0 : stack->size;
}

bool int_stack_is_empty(const IntStack *stack) {
    return int_stack_size(stack) == 0;
}
