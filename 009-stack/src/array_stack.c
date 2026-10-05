#include "array_stack.h"

#include <stdint.h>
#include <stdlib.h>

struct ArrayStack {
    int *data;
    size_t size;
    size_t capacity;
};

ArrayStack *array_stack_create(void) {
    return calloc(1, sizeof(ArrayStack));
}

void array_stack_free(ArrayStack *stack) {
    if (stack == NULL) return;
    free(stack->data);
    free(stack);
}

size_t array_stack_size(const ArrayStack *stack) {
    return stack == NULL ? 0 : stack->size;
}

bool array_stack_is_empty(const ArrayStack *stack) {
    return array_stack_size(stack) == 0;
}

static bool reserve(ArrayStack *stack, size_t need) {
    if (need <= stack->capacity) return true;

    size_t capacity = stack->capacity == 0 ? 8 : stack->capacity;
    while (capacity < need) {
        if (capacity > SIZE_MAX / 2) {
            capacity = need;
            break;
        }
        capacity *= 2;
    }

    if (capacity > SIZE_MAX / sizeof *stack->data) return false;

    int *new_data = realloc(stack->data, capacity * sizeof *stack->data);
    if (new_data == NULL) return false;

    stack->data = new_data;
    stack->capacity = capacity;
    return true;
}

bool array_stack_push(ArrayStack *stack, int value) {
    if (stack == NULL || stack->size == SIZE_MAX) return false;
    if (!reserve(stack, stack->size + 1)) return false;

    stack->data[stack->size++] = value;
    return true;
}

bool array_stack_pop(ArrayStack *stack, int *out) {
    if (stack == NULL || stack->size == 0) return false;

    const int value = stack->data[stack->size - 1];
    --stack->size;
    if (out != NULL) *out = value;
    return true;
}

bool array_stack_peek(const ArrayStack *stack, int *out) {
    if (stack == NULL || stack->size == 0 || out == NULL) return false;
    *out = stack->data[stack->size - 1];
    return true;
}

bool array_stack_validate(const ArrayStack *stack) {
    if (stack == NULL) return false;
    if (stack->size > stack->capacity) return false;
    if (stack->capacity == 0 && stack->data != NULL) return false;
    if (stack->capacity > 0 && stack->data == NULL) return false;
    return true;
}
