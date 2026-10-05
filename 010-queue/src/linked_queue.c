#include "linked_queue.h"

#include <stdlib.h>

typedef struct QueueNode {
    int value;
    struct QueueNode *next;
} QueueNode;

struct LinkedQueue {
    QueueNode *head;
    QueueNode *tail;
    size_t size;
};

LinkedQueue *linked_queue_create(void) {
    return calloc(1, sizeof(LinkedQueue));
}

void linked_queue_free(LinkedQueue *queue) {
    if (queue == NULL) return;

    QueueNode *node = queue->head;
    while (node != NULL) {
        QueueNode *next = node->next;
        free(node);
        node = next;
    }

    free(queue);
}

size_t linked_queue_size(const LinkedQueue *queue) {
    return queue == NULL ? 0 : queue->size;
}

bool linked_queue_is_empty(const LinkedQueue *queue) {
    return linked_queue_size(queue) == 0;
}

bool linked_queue_enqueue(LinkedQueue *queue, int value) {
    if (queue == NULL) return false;

    QueueNode *node = malloc(sizeof *node);
    if (node == NULL) return false;

    node->value = value;
    node->next = NULL;

    if (queue->tail != NULL) {
        queue->tail->next = node;
    } else {
        queue->head = node;
    }

    queue->tail = node;
    ++queue->size;
    return true;
}

bool linked_queue_dequeue(LinkedQueue *queue, int *out) {
    if (queue == NULL || queue->head == NULL) return false;

    QueueNode *victim = queue->head;
    queue->head = victim->next;

    if (out != NULL) *out = victim->value;

    free(victim);
    --queue->size;

    if (queue->size == 0) queue->tail = NULL;
    return true;
}

bool linked_queue_peek(const LinkedQueue *queue, int *out) {
    if (queue == NULL || queue->head == NULL || out == NULL) return false;
    *out = queue->head->value;
    return true;
}

bool linked_queue_validate(const LinkedQueue *queue) {
    if (queue == NULL) return false;

    if (queue->size == 0) {
        return queue->head == NULL && queue->tail == NULL;
    }

    if (queue->head == NULL || queue->tail == NULL || queue->tail->next != NULL) {
        return false;
    }

    size_t count = 0;
    const QueueNode *node = queue->head;
    const QueueNode *last = NULL;

    while (node != NULL && count <= queue->size) {
        last = node;
        node = node->next;
        ++count;
    }

    return node == NULL && count == queue->size && last == queue->tail;
}
