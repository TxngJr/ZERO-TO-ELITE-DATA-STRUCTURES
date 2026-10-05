#include "circular_queue.h"
#include "linked_queue.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void test_randomized(void) {
    enum { MAX = 1024, STEPS = 12000 };

    CircularQueue *c = circular_queue_create();
    LinkedQueue *l = linked_queue_create();
    assert(c != NULL && l != NULL);

    int ref[MAX];
    size_t size = 0;
    uint32_t rng = 0xA11CE55u;

    for (int step = 0; step < STEPS; ++step) {
        const uint32_t r = next_rng(&rng);

        if ((r & 1U) == 0U && size < MAX) {
            const int value = (int)next_rng(&rng);

            assert(circular_queue_enqueue(c, value));
            assert(linked_queue_enqueue(l, value));
            ref[size++] = value;
        } else if (size > 0) {
            int a = 0;
            int b = 0;

            assert(circular_queue_peek(c, &a));
            assert(linked_queue_peek(l, &b));
            assert(a == ref[0]);
            assert(b == ref[0]);

            assert(circular_queue_dequeue(c, &a));
            assert(linked_queue_dequeue(l, &b));
            assert(a == ref[0]);
            assert(b == ref[0]);

            memmove(ref, ref + 1, (size - 1) * sizeof ref[0]);
            --size;
        }

        assert(circular_queue_validate(c));
        assert(linked_queue_validate(l));
        assert(circular_queue_size(c) == size);
        assert(linked_queue_size(l) == size);
    }

    circular_queue_free(c);
    linked_queue_free(l);
}

static void test_wrap_and_growth(void) {
    CircularQueue *q = circular_queue_create();
    assert(q != NULL);

    for (int i = 0; i < 32; ++i) assert(circular_queue_enqueue(q, i));

    for (int i = 0; i < 20; ++i) {
        int out = -1;
        assert(circular_queue_dequeue(q, &out));
        assert(out == i);
    }

    for (int i = 32; i < 100; ++i) assert(circular_queue_enqueue(q, i));

    for (int expected = 20; expected < 100; ++expected) {
        int out = -1;
        assert(circular_queue_dequeue(q, &out));
        assert(out == expected);
    }

    assert(circular_queue_is_empty(q));
    assert(circular_queue_validate(q));
    circular_queue_free(q);
}

static void test_underflow(void) {
    CircularQueue *c = circular_queue_create();
    LinkedQueue *l = linked_queue_create();
    assert(c != NULL && l != NULL);

    int out = 0;
    assert(!circular_queue_dequeue(c, &out));
    assert(!linked_queue_dequeue(l, &out));
    assert(!circular_queue_peek(c, &out));
    assert(!linked_queue_peek(l, &out));

    circular_queue_free(c);
    linked_queue_free(l);
}

int main(void) {
    test_randomized();
    test_wrap_and_growth();
    test_underflow();

    puts("queue tests passed");
    return 0;
}
