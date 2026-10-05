#ifndef MAX_PRIORITY_QUEUE_H
#define MAX_PRIORITY_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int value;
    int priority;
} PriorityItem;

typedef struct MaxPriorityQueue MaxPriorityQueue;

MaxPriorityQueue *max_pq_create(void);
void max_pq_free(MaxPriorityQueue *queue);
size_t max_pq_size(const MaxPriorityQueue *queue);
bool max_pq_is_empty(const MaxPriorityQueue *queue);

bool max_pq_push(MaxPriorityQueue *queue, PriorityItem item);
bool max_pq_peek(const MaxPriorityQueue *queue, PriorityItem *out);
bool max_pq_pop(MaxPriorityQueue *queue, PriorityItem *out);
bool max_pq_validate(const MaxPriorityQueue *queue);

#endif
