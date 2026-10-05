#include "max_priority_queue.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    MaxPriorityQueue *q = max_pq_create();
    assert(q != NULL);

    assert(max_pq_push(q, (PriorityItem){.value=100, .priority=2}));
    assert(max_pq_push(q, (PriorityItem){.value=200, .priority=10}));
    assert(max_pq_push(q, (PriorityItem){.value=300, .priority=5}));

    puts("pop by descending priority:");
    while (!max_pq_is_empty(q)) {
        PriorityItem item;
        assert(max_pq_pop(q, &item));
        printf("value=%d priority=%d\n", item.value, item.priority);
    }

    max_pq_free(q);
    return 0;
}
