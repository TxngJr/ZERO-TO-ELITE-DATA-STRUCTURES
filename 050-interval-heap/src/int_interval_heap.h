#ifndef INT_INTERVAL_HEAP_H
#define INT_INTERVAL_HEAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct IntIntervalHeap IntIntervalHeap;

IntIntervalHeap *int_interval_heap_create(void);
void int_interval_heap_free(IntIntervalHeap *heap);

size_t int_interval_heap_size(const IntIntervalHeap *heap);
bool int_interval_heap_is_empty(const IntIntervalHeap *heap);

bool int_interval_heap_insert(
    IntIntervalHeap *heap,
    int64_t value
);

bool int_interval_heap_peek_min(
    const IntIntervalHeap *heap,
    int64_t *out_value
);
bool int_interval_heap_peek_max(
    const IntIntervalHeap *heap,
    int64_t *out_value
);

bool int_interval_heap_pop_min(
    IntIntervalHeap *heap,
    int64_t *out_value
);
bool int_interval_heap_pop_max(
    IntIntervalHeap *heap,
    int64_t *out_value
);

bool int_interval_heap_validate(
    const IntIntervalHeap *heap
);

#endif
