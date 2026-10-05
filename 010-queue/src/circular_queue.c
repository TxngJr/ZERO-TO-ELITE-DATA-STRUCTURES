#include "circular_queue.h"

#include <stdint.h>
#include <stdlib.h>

struct CircularQueue {
    int *data;
    size_t capacity;
    size_t head;
    size_t size;
};

static size_t wrapped_add(size_t index, size_t offset, size_t capacity) {
    if (capacity == 0) return 0;
    offset %= capacity;
    const size_t room = capacity - index;
    return offset < room ? index + offset : offset - room;
}

static bool grow(CircularQueue *queue, size_t need) {
    if (need <= queue->capacity) return true;

    size_t new_capacity = queue->capacity == 0 ? 8 : queue->capacity;
    while (new_capacity < need) {
        if (new_capacity > SIZE_MAX / 2) {
            new_capacity = need;
            break;
        }
        new_capacity *= 2;
    }

    if (new_capacity > SIZE_MAX / sizeof *queue->data) return false;

    int *new_data = malloc(new_capacity * sizeof *new_data);
    if (new_data == NULL) return false;

    for (size_t i = 0; i < queue->size; ++i) {
        new_data[i] = queue->data[wrapped_add(queue->head, i, queue->capacity)];
    }

    free(queue->data);
    queue->data = new_data;
    queue->capacity = new_capacity;
    queue->head = 0;
    return true;
}

CircularQueue *circular_queue_create(void) {
    return calloc(1, sizeof(CircularQueue));
}

void circular_queue_free(CircularQueue *queue) {
    if (queue == NULL) return;
    free(queue->data);
    free(queue);
}

size_t circular_queue_size(const CircularQueue *queue) {
    return queue == NULL ? 0 : queue->size;
}

bool circular_queue_is_empty(const CircularQueue *queue) {
    return circular_queue_size(queue) == 0;
}

bool circular_queue_enqueue(CircularQueue *queue, int value) {
    if (queue == NULL || queue->size == SIZE_MAX) return false;
    if (!grow(queue, queue->size + 1)) return false;

    const size_t tail = wrapped_add(queue->head, queue->size, queue->capacity);
    queue->data[tail] = value;
    ++queue->size;
    return true;
}

bool circular_queue_dequeue(CircularQueue *queue, int *out) {
    if (queue == NULL || queue->size == 0) return false;

    const int value = queue->data[queue->head];
    queue->head = wrapped_add(queue->head, 1, queue->capacity);
    --queue->size;

    if (queue->size == 0) queue->head = 0;
    if (out != NULL) *out = value;
    return true;
}

bool circular_queue_peek(const CircularQueue *queue, int *out) {
    if (queue == NULL || queue->size == 0 || out == NULL) return false;
    *out = queue->data[queue->head];
    return true;
}

bool circular_queue_validate(const CircularQueue *queue) {
    if (queue == NULL) return false;
    if (queue->size > queue->capacity) return false;
    if (queue->capacity == 0) {
        return queue->data == NULL && queue->size == 0 && queue->head == 0;
    }
    if (queue->data == NULL) return false;
    if (queue->head >= queue->capacity) return false;
    return true;
}
