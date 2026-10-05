#ifndef ARRAY_STACK_H
#define ARRAY_STACK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct ArrayStack ArrayStack;

ArrayStack *array_stack_create(void);
void array_stack_free(ArrayStack *stack);
size_t array_stack_size(const ArrayStack *stack);
bool array_stack_is_empty(const ArrayStack *stack);
bool array_stack_push(ArrayStack *stack, int value);
bool array_stack_pop(ArrayStack *stack, int *out);
bool array_stack_peek(const ArrayStack *stack, int *out);
bool array_stack_validate(const ArrayStack *stack);

#endif
