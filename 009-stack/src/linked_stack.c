#include "linked_stack.h"

#include <stdlib.h>

typedef struct StackNode {
    int value;
    struct StackNode *next;
} StackNode;

struct LinkedStack {
    StackNode *top;
    size_t size;
};

LinkedStack *linked_stack_create(void) {
    return calloc(1, sizeof(LinkedStack));
}

void linked_stack_free(LinkedStack *stack) {
    if (stack == NULL) return;
    StackNode *node = stack->top;
    while (node != NULL) {
        StackNode *next = node->next;
        free(node);
        node = next;
    }
    free(stack);
}

size_t linked_stack_size(const LinkedStack *stack) {
    return stack == NULL ? 0 : stack->size;
}

bool linked_stack_is_empty(const LinkedStack *stack) {
    return linked_stack_size(stack) == 0;
}

bool linked_stack_push(LinkedStack *stack, int value) {
    if (stack == NULL) return false;
    StackNode *node = malloc(sizeof *node);
    if (node == NULL) return false;

    node->value = value;
    node->next = stack->top;
    stack->top = node;
    ++stack->size;
    return true;
}

bool linked_stack_pop(LinkedStack *stack, int *out) {
    if (stack == NULL || stack->top == NULL) return false;

    StackNode *victim = stack->top;
    stack->top = victim->next;
    if (out != NULL) *out = victim->value;
    free(victim);
    --stack->size;
    return true;
}

bool linked_stack_peek(const LinkedStack *stack, int *out) {
    if (stack == NULL || stack->top == NULL || out == NULL) return false;
    *out = stack->top->value;
    return true;
}

bool linked_stack_validate(const LinkedStack *stack) {
    if (stack == NULL) return false;
    if ((stack->size == 0) != (stack->top == NULL)) return false;

    size_t count = 0;
    const StackNode *node = stack->top;
    while (node != NULL && count <= stack->size) {
        node = node->next;
        ++count;
    }
    return node == NULL && count == stack->size;
}
