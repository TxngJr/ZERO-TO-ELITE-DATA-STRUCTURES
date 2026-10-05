#ifndef INT_DEQUE_H
#define INT_DEQUE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntDeque IntDeque;

IntDeque *int_deque_create(void);
void int_deque_free(IntDeque *deque);
size_t int_deque_size(const IntDeque *deque);
bool int_deque_is_empty(const IntDeque *deque);

bool int_deque_push_front(IntDeque *deque, int value);
bool int_deque_push_back(IntDeque *deque, int value);
bool int_deque_pop_front(IntDeque *deque, int *out);
bool int_deque_pop_back(IntDeque *deque, int *out);
bool int_deque_front(const IntDeque *deque, int *out);
bool int_deque_back(const IntDeque *deque, int *out);
bool int_deque_get(const IntDeque *deque, size_t index, int *out);
bool int_deque_validate(const IntDeque *deque);

#endif
