#ifndef INT_FIBONACCI_HEAP_H
#define INT_FIBONACCI_HEAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntFibonacciNode IntFibonacciNode;
typedef struct IntFibonacciHeap IntFibonacciHeap;

IntFibonacciHeap *int_fib_heap_create(void);
void int_fib_heap_free(IntFibonacciHeap *heap);

size_t int_fib_heap_size(const IntFibonacciHeap *heap);
IntFibonacciNode *int_fib_heap_insert(IntFibonacciHeap *heap, int key);
bool int_fib_heap_peek_min(const IntFibonacciHeap *heap, int *out_key);
bool int_fib_heap_extract_min(IntFibonacciHeap *heap, int *out_key);
bool int_fib_heap_meld(IntFibonacciHeap *destination, IntFibonacciHeap *source);

bool int_fib_heap_decrease_key(IntFibonacciHeap *heap, IntFibonacciNode *node, int new_key);
bool int_fib_heap_delete_handle(IntFibonacciHeap *heap, IntFibonacciNode *node);

int int_fib_node_key(const IntFibonacciNode *node);
bool int_fib_heap_validate(const IntFibonacciHeap *heap);

#endif
