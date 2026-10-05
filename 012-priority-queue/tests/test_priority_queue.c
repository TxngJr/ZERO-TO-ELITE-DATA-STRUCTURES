#include "max_priority_queue.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    PriorityItem items[1024];
    size_t size;
} Reference;

static size_t ref_max_index(const Reference *ref) {
    size_t best = 0;
    for (size_t i = 1; i < ref->size; ++i) {
        if (ref->items[i].priority > ref->items[best].priority) {
            best = i;
        }
    }
    return best;
}

static void ref_remove_index(Reference *ref, size_t index) {
    ref->items[index] = ref->items[ref->size - 1];
    --ref->size;
}

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void test_known_sequence(void) {
    MaxPriorityQueue *q = max_pq_create();
    assert(q != NULL);

    const PriorityItem input[] = {
        {1, 4}, {2, 10}, {3, 7}, {4, 20}, {5, 3}, {6, 15}
    };

    for (size_t i = 0; i < sizeof input / sizeof input[0]; ++i) {
        assert(max_pq_push(q, input[i]));
        assert(max_pq_validate(q));
    }

    const int expected_priorities[] = {20, 15, 10, 7, 4, 3};

    for (size_t i = 0; i < 6; ++i) {
        PriorityItem item;
        assert(max_pq_pop(q, &item));
        assert(item.priority == expected_priorities[i]);
        assert(max_pq_validate(q));
    }

    assert(max_pq_is_empty(q));
    max_pq_free(q);
}

static void test_randomized_differential(void) {
    enum { STEPS = 15000 };

    MaxPriorityQueue *q = max_pq_create();
    assert(q != NULL);

    Reference ref = {.size=0};
    uint32_t rng = 0x51504F51u;
    int next_value = 1;

    for (int step = 0; step < STEPS; ++step) {
        const uint32_t r = next_rng(&rng);

        if ((r & 1U) == 0U && ref.size < 1024) {
            PriorityItem item = {
                .value = next_value++,
                .priority = (int)(next_rng(&rng) % 101U) - 50
            };

            assert(max_pq_push(q, item));
            ref.items[ref.size++] = item;
        } else if (ref.size > 0) {
            const size_t best = ref_max_index(&ref);
            const int max_priority = ref.items[best].priority;

            PriorityItem heap_item;
            assert(max_pq_pop(q, &heap_item));
            assert(heap_item.priority == max_priority);

            size_t match = ref.size;
            for (size_t i = 0; i < ref.size; ++i) {
                if (ref.items[i].value == heap_item.value &&
                    ref.items[i].priority == heap_item.priority) {
                    match = i;
                    break;
                }
            }

            assert(match < ref.size);
            ref_remove_index(&ref, match);
        }

        assert(max_pq_validate(q));
        assert(max_pq_size(q) == ref.size);

        if (ref.size > 0) {
            const int max_priority = ref.items[ref_max_index(&ref)].priority;
            PriorityItem top;
            assert(max_pq_peek(q, &top));
            assert(top.priority == max_priority);
        }
    }

    max_pq_free(q);
}

static void test_equal_priorities(void) {
    MaxPriorityQueue *q = max_pq_create();
    assert(q != NULL);

    for (int i = 0; i < 20; ++i) {
        assert(max_pq_push(q, (PriorityItem){.value=i, .priority=7}));
    }

    bool seen[20] = {false};

    for (int i = 0; i < 20; ++i) {
        PriorityItem item;
        assert(max_pq_pop(q, &item));
        assert(item.priority == 7);
        assert(item.value >= 0 && item.value < 20);
        assert(!seen[item.value]);
        seen[item.value] = true;
    }

    max_pq_free(q);
}

int main(void) {
    test_known_sequence();
    test_randomized_differential();
    test_equal_priorities();

    puts("priority queue tests passed");
    return 0;
}
