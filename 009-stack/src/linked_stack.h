#ifndef LINKED_STACK_H
#define LINKED_STACK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct LinkedStack LinkedStack;

LinkedStack *linked_stack_create(void);
void linked_stack_free(LinkedStack *stack);
size_t linked_stack_size(const LinkedStack *stack);
bool linked_stack_is_empty(const LinkedStack *stack);
bool linked_stack_push(LinkedStack *stack, int value);
bool linked_stack_pop(LinkedStack *stack, int *out);
bool linked_stack_peek(const LinkedStack *stack, int *out);
bool linked_stack_validate(const LinkedStack *stack);

#endif
