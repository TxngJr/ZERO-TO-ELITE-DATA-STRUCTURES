#ifndef INT_STACK_H
#define INT_STACK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntStack IntStack;

/* Returns a new empty stack, or NULL if allocation fails. */
IntStack *int_stack_create(void);

/* Safe to call with NULL. Frees all memory owned by the stack. */
void int_stack_free(IntStack *stack);

/* Pushes value. Returns false on allocation failure or NULL stack. */
bool int_stack_push(IntStack *stack, int value);

/* Removes the top item into *out. Returns false if empty/invalid. */
bool int_stack_pop(IntStack *stack, int *out);

/* Reads the top item without removing it. */
bool int_stack_peek(const IntStack *stack, int *out);

size_t int_stack_size(const IntStack *stack);
bool int_stack_is_empty(const IntStack *stack);

#endif
