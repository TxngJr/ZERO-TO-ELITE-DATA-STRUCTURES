#ifndef LINKED_QUEUE_H
#define LINKED_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct LinkedQueue LinkedQueue;

LinkedQueue *linked_queue_create(void);
void linked_queue_free(LinkedQueue *queue);
size_t linked_queue_size(const LinkedQueue *queue);
bool linked_queue_is_empty(const LinkedQueue *queue);
bool linked_queue_enqueue(LinkedQueue *queue, int value);
bool linked_queue_dequeue(LinkedQueue *queue, int *out);
bool linked_queue_peek(const LinkedQueue *queue, int *out);
bool linked_queue_validate(const LinkedQueue *queue);

#endif
