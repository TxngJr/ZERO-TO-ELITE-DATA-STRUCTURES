#include "circular_queue.h"
#include "linked_queue.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    CircularQueue *c = circular_queue_create();
    LinkedQueue *l = linked_queue_create();
    assert(c != NULL && l != NULL);

    for (int i = 1; i <= 5; ++i) {
        assert(circular_queue_enqueue(c, i * 10));
        assert(linked_queue_enqueue(l, i * 10));
    }

    puts("dequeue order:");
    while (!circular_queue_is_empty(c)) {
        int a = 0;
        int b = 0;
        assert(circular_queue_dequeue(c, &a));
        assert(linked_queue_dequeue(l, &b));
        assert(a == b);
        printf("%d\n", a);
    }

    circular_queue_free(c);
    linked_queue_free(l);
    return 0;
}
