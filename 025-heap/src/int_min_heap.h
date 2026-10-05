#ifndef INT_MIN_HEAP_H
#define INT_MIN_HEAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntMinHeap IntMinHeap;

IntMinHeap *int_min_heap_create(void);
IntMinHeap *int_min_heap_build(const int *values, size_t count);
void int_min_heap_free(IntMinHeap *heap);

size_t int_min_heap_size(const IntMinHeap *heap);
bool int_min_heap_push(IntMinHeap *heap, int value);
bool int_min_heap_peek(const IntMinHeap *heap, int *out_value);
bool int_min_heap_pop(IntMinHeap *heap, int *out_value);
bool int_min_heap_replace(IntMinHeap *heap, size_t index, int value);
bool int_min_heap_validate(const IntMinHeap *heap);

#endif
