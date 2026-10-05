#ifndef INT_DARY_HEAP_H
#define INT_DARY_HEAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntDaryHeap IntDaryHeap;

IntDaryHeap *int_dary_heap_create(size_t arity);
IntDaryHeap *int_dary_heap_build(size_t arity, const int *values, size_t count);
void int_dary_heap_free(IntDaryHeap *heap);

size_t int_dary_heap_size(const IntDaryHeap *heap);
size_t int_dary_heap_arity(const IntDaryHeap *heap);

bool int_dary_heap_push(IntDaryHeap *heap, int value);
bool int_dary_heap_peek(const IntDaryHeap *heap, int *out_value);
bool int_dary_heap_pop(IntDaryHeap *heap, int *out_value);
bool int_dary_heap_validate(const IntDaryHeap *heap);

#endif
