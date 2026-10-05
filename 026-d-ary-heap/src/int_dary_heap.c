#include "int_dary_heap.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct IntDaryHeap {
    int *data;
    size_t size;
    size_t capacity;
    size_t arity;
};

static void swap_int(int *a, int *b) {
    const int tmp = *a;
    *a = *b;
    *b = tmp;
}

static size_t parent_index(const IntDaryHeap *heap, size_t index) {
    return (index - 1) / heap->arity;
}

static bool first_child(
    const IntDaryHeap *heap,
    size_t index,
    size_t *out_first
) {
    if (heap->size < 2 || out_first == NULL) return false;

    if (index > (heap->size - 2) / heap->arity) return false;

    *out_first = index * heap->arity + 1;
    return true;
}

static void sift_up(IntDaryHeap *heap, size_t index) {
    while (index > 0) {
        const size_t parent = parent_index(heap, index);

        if (heap->data[parent] <= heap->data[index]) break;

        swap_int(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

static void sift_down(IntDaryHeap *heap, size_t index) {
    for (;;) {
        size_t first = 0;
        if (!first_child(heap, index, &first)) return;

        const size_t available = heap->size - first;
        const size_t child_count =
            heap->arity < available ? heap->arity : available;

        size_t smallest = first;

        for (size_t offset = 1; offset < child_count; ++offset) {
            const size_t child = first + offset;

            if (heap->data[child] < heap->data[smallest]) {
                smallest = child;
            }
        }

        if (heap->data[index] <= heap->data[smallest]) return;

        swap_int(&heap->data[index], &heap->data[smallest]);
        index = smallest;
    }
}

static bool reserve(IntDaryHeap *heap, size_t needed) {
    if (needed <= heap->capacity) return true;

    size_t capacity = heap->capacity == 0 ? 8 : heap->capacity;

    while (capacity < needed) {
        if (capacity > SIZE_MAX / 2) {
            capacity = needed;
            break;
        }
        capacity *= 2;
    }

    if (capacity > SIZE_MAX / sizeof *heap->data) return false;

    int *next = realloc(heap->data, capacity * sizeof *next);
    if (next == NULL) return false;

    heap->data = next;
    heap->capacity = capacity;
    return true;
}

IntDaryHeap *int_dary_heap_create(size_t arity) {
    if (arity < 2) return NULL;

    IntDaryHeap *heap = calloc(1, sizeof *heap);
    if (heap == NULL) return NULL;

    heap->arity = arity;
    return heap;
}

IntDaryHeap *int_dary_heap_build(
    size_t arity,
    const int *values,
    size_t count
) {
    if (arity < 2 || (count > 0 && values == NULL)) return NULL;

    IntDaryHeap *heap = int_dary_heap_create(arity);
    if (heap == NULL) return NULL;

    if (!reserve(heap, count)) {
        int_dary_heap_free(heap);
        return NULL;
    }

    if (count > 0) {
        memcpy(heap->data, values, count * sizeof *values);
    }

    heap->size = count;

    if (count >= 2) {
        const size_t last_internal = (count - 2) / arity;

        for (size_t i = last_internal + 1; i > 0; --i) {
            sift_down(heap, i - 1);
        }
    }

    return heap;
}

void int_dary_heap_free(IntDaryHeap *heap) {
    if (heap == NULL) return;
    free(heap->data);
    free(heap);
}

size_t int_dary_heap_size(const IntDaryHeap *heap) {
    return heap == NULL ? 0 : heap->size;
}

size_t int_dary_heap_arity(const IntDaryHeap *heap) {
    return heap == NULL ? 0 : heap->arity;
}

bool int_dary_heap_push(IntDaryHeap *heap, int value) {
    if (heap == NULL || heap->arity < 2 || heap->size == SIZE_MAX) return false;

    if (!reserve(heap, heap->size + 1)) return false;

    const size_t index = heap->size++;
    heap->data[index] = value;
    sift_up(heap, index);

    return true;
}

bool int_dary_heap_peek(const IntDaryHeap *heap, int *out_value) {
    if (heap == NULL || heap->size == 0 || out_value == NULL) return false;

    *out_value = heap->data[0];
    return true;
}

bool int_dary_heap_pop(IntDaryHeap *heap, int *out_value) {
    if (heap == NULL || heap->size == 0) return false;

    const int result = heap->data[0];
    --heap->size;

    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        sift_down(heap, 0);
    }

    if (out_value != NULL) *out_value = result;
    return true;
}

bool int_dary_heap_validate(const IntDaryHeap *heap) {
    if (heap == NULL || heap->arity < 2) return false;
    if (heap->size > heap->capacity) return false;
    if (heap->capacity > 0 && heap->data == NULL) return false;

    for (size_t i = 1; i < heap->size; ++i) {
        if (heap->data[parent_index(heap, i)] > heap->data[i]) return false;
    }

    return true;
}
