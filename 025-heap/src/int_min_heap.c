#include "int_min_heap.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct IntMinHeap {
    int *data;
    size_t size;
    size_t capacity;
};

static void swap_int(int *a, int *b) {
    const int tmp = *a;
    *a = *b;
    *b = tmp;
}

static size_t parent_index(size_t index) {
    return (index - 1) / 2;
}

static void sift_up(IntMinHeap *heap, size_t index) {
    while (index > 0) {
        const size_t parent = parent_index(index);

        if (heap->data[parent] <= heap->data[index]) break;

        swap_int(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

static void sift_down(IntMinHeap *heap, size_t index) {
    if (heap->size < 2) return;

    while (index <= (heap->size - 2) / 2) {
        const size_t left = index * 2 + 1;
        const size_t right = left + 1;
        size_t smallest = left;

        if (right < heap->size &&
            heap->data[right] < heap->data[left]) {
            smallest = right;
        }

        if (heap->data[index] <= heap->data[smallest]) break;

        swap_int(&heap->data[index], &heap->data[smallest]);
        index = smallest;
    }
}

static bool reserve(IntMinHeap *heap, size_t needed) {
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

IntMinHeap *int_min_heap_create(void) {
    return calloc(1, sizeof(IntMinHeap));
}

IntMinHeap *int_min_heap_build(const int *values, size_t count) {
    if (count > 0 && values == NULL) return NULL;

    IntMinHeap *heap = int_min_heap_create();
    if (heap == NULL) return NULL;

    if (!reserve(heap, count)) {
        int_min_heap_free(heap);
        return NULL;
    }

    if (count > 0) {
        memcpy(heap->data, values, count * sizeof *values);
    }

    heap->size = count;

    for (size_t i = count / 2; i > 0; --i) {
        sift_down(heap, i - 1);
    }

    return heap;
}

void int_min_heap_free(IntMinHeap *heap) {
    if (heap == NULL) return;
    free(heap->data);
    free(heap);
}

size_t int_min_heap_size(const IntMinHeap *heap) {
    return heap == NULL ? 0 : heap->size;
}

bool int_min_heap_push(IntMinHeap *heap, int value) {
    if (heap == NULL || heap->size == SIZE_MAX) return false;

    if (!reserve(heap, heap->size + 1)) return false;

    const size_t index = heap->size++;
    heap->data[index] = value;
    sift_up(heap, index);
    return true;
}

bool int_min_heap_peek(const IntMinHeap *heap, int *out_value) {
    if (heap == NULL || heap->size == 0 || out_value == NULL) return false;

    *out_value = heap->data[0];
    return true;
}

bool int_min_heap_pop(IntMinHeap *heap, int *out_value) {
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

bool int_min_heap_replace(IntMinHeap *heap, size_t index, int value) {
    if (heap == NULL || index >= heap->size) return false;

    const int old = heap->data[index];
    heap->data[index] = value;

    if (value < old) sift_up(heap, index);
    else if (value > old) sift_down(heap, index);

    return true;
}

bool int_min_heap_validate(const IntMinHeap *heap) {
    if (heap == NULL) return false;
    if (heap->size > heap->capacity) return false;
    if (heap->capacity > 0 && heap->data == NULL) return false;

    for (size_t i = 1; i < heap->size; ++i) {
        if (heap->data[parent_index(i)] > heap->data[i]) return false;
    }

    return true;
}
