#include "max_priority_queue.h"

#include <stdint.h>
#include <stdlib.h>

struct MaxPriorityQueue {
    PriorityItem *data;
    size_t size;
    size_t capacity;
};

static void swap_items(PriorityItem *a, PriorityItem *b) {
    const PriorityItem tmp = *a;
    *a = *b;
    *b = tmp;
}

static bool reserve(MaxPriorityQueue *queue, size_t need) {
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

    PriorityItem *new_data = realloc(
        queue->data,
        new_capacity * sizeof *new_data
    );

    if (new_data == NULL) return false;

    queue->data = new_data;
    queue->capacity = new_capacity;
    return true;
}

static void sift_up(MaxPriorityQueue *queue, size_t index) {
    while (index > 0) {
        const size_t parent = (index - 1) / 2;

        if (queue->data[parent].priority >= queue->data[index].priority) {
            break;
        }

        swap_items(&queue->data[parent], &queue->data[index]);
        index = parent;
    }
}

static void sift_down(MaxPriorityQueue *queue, size_t index) {
    while (index < queue->size / 2) {
        const size_t left = index * 2 + 1;
        const size_t right = left + 1;
        size_t larger = left;

        if (right < queue->size &&
            queue->data[right].priority > queue->data[left].priority) {
            larger = right;
        }

        if (queue->data[index].priority >= queue->data[larger].priority) {
            break;
        }

        swap_items(&queue->data[index], &queue->data[larger]);
        index = larger;
    }
}

MaxPriorityQueue *max_pq_create(void) {
    return calloc(1, sizeof(MaxPriorityQueue));
}

void max_pq_free(MaxPriorityQueue *queue) {
    if (queue == NULL) return;
    free(queue->data);
    free(queue);
}

size_t max_pq_size(const MaxPriorityQueue *queue) {
    return queue == NULL ? 0 : queue->size;
}

bool max_pq_is_empty(const MaxPriorityQueue *queue) {
    return max_pq_size(queue) == 0;
}

bool max_pq_push(MaxPriorityQueue *queue, PriorityItem item) {
    if (queue == NULL || queue->size == SIZE_MAX) return false;
    if (!reserve(queue, queue->size + 1)) return false;

    const size_t index = queue->size;
    queue->data[index] = item;
    ++queue->size;
    sift_up(queue, index);
    return true;
}

bool max_pq_peek(const MaxPriorityQueue *queue, PriorityItem *out) {
    if (queue == NULL || queue->size == 0 || out == NULL) return false;
    *out = queue->data[0];
    return true;
}

bool max_pq_pop(MaxPriorityQueue *queue, PriorityItem *out) {
    if (queue == NULL || queue->size == 0) return false;

    const PriorityItem result = queue->data[0];
    --queue->size;

    if (queue->size > 0) {
        queue->data[0] = queue->data[queue->size];
        sift_down(queue, 0);
    }

    if (out != NULL) *out = result;
    return true;
}

bool max_pq_validate(const MaxPriorityQueue *queue) {
    if (queue == NULL) return false;
    if (queue->size > queue->capacity) return false;
    if (queue->capacity == 0 && queue->data != NULL) return false;
    if (queue->capacity > 0 && queue->data == NULL) return false;

    for (size_t i = 1; i < queue->size; ++i) {
        const size_t parent = (i - 1) / 2;
        if (queue->data[parent].priority < queue->data[i].priority) {
            return false;
        }
    }

    return true;
}
