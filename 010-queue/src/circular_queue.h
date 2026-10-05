#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct CircularQueue CircularQueue;

CircularQueue *circular_queue_create(void);
void circular_queue_free(CircularQueue *queue);
size_t circular_queue_size(const CircularQueue *queue);
bool circular_queue_is_empty(const CircularQueue *queue);
bool circular_queue_enqueue(CircularQueue *queue, int value);
bool circular_queue_dequeue(CircularQueue *queue, int *out);
bool circular_queue_peek(const CircularQueue *queue, int *out);
bool circular_queue_validate(const CircularQueue *queue);

#endif
