#include "concurrent_mpmc_ring.h"
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    _Atomic size_t sequence;
    int data;
} Cell;

struct ConcurrentMpmcRing {
    size_t capacity;
    size_t mask;
    Cell *cells;
    _Atomic size_t enqueue_pos;
    _Atomic size_t dequeue_pos;
};

static bool is_power_of_two(size_t x) {
    return x >= 2U && (x & (x - 1U)) == 0U;
}

ConcurrentMpmcRing *cmr_create(size_t capacity) {
    if (!is_power_of_two(capacity) ||
        capacity > SIZE_MAX / sizeof(Cell))
        return NULL;

    ConcurrentMpmcRing *ring = calloc(1, sizeof(*ring));
    if (!ring) return NULL;

    ring->cells = malloc(capacity * sizeof(*ring->cells));
    if (!ring->cells) {
        free(ring);
        return NULL;
    }

    ring->capacity = capacity;
    ring->mask = capacity - 1U;

    for (size_t i = 0; i < capacity; ++i)
        atomic_init(&ring->cells[i].sequence, i);

    atomic_init(&ring->enqueue_pos, 0U);
    atomic_init(&ring->dequeue_pos, 0U);
    return ring;
}

void cmr_free(ConcurrentMpmcRing *ring) {
    if (!ring) return;
    free(ring->cells);
    free(ring);
}

bool cmr_platform_lock_free(const ConcurrentMpmcRing *ring) {
    if (!ring) return false;

    if (!atomic_is_lock_free(&ring->enqueue_pos) ||
        !atomic_is_lock_free(&ring->dequeue_pos))
        return false;

    for (size_t i = 0; i < ring->capacity; ++i) {
        if (!atomic_is_lock_free(&ring->cells[i].sequence))
            return false;
    }
    return true;
}

size_t cmr_capacity(const ConcurrentMpmcRing *ring) {
    return ring ? ring->capacity : 0U;
}

bool cmr_try_push(ConcurrentMpmcRing *ring, int value,
                  bool *out_pushed) {
    if (!ring) return false;

    size_t pos = atomic_load_explicit(
        &ring->enqueue_pos, memory_order_relaxed);

    for (;;) {
        if (pos > SIZE_MAX - ring->capacity - 1U)
            return false;

        Cell *cell = &ring->cells[pos & ring->mask];
        size_t sequence = atomic_load_explicit(
            &cell->sequence, memory_order_acquire);

        if (sequence == pos) {
            size_t expected = pos;
            if (atomic_compare_exchange_weak_explicit(
                    &ring->enqueue_pos, &expected, pos + 1U,
                    memory_order_relaxed, memory_order_relaxed)) {
                cell->data = value;
                atomic_store_explicit(
                    &cell->sequence, pos + 1U, memory_order_release);
                if (out_pushed) *out_pushed = true;
                return true;
            }
            pos = expected;
        } else if (sequence < pos) {
            if (out_pushed) *out_pushed = false;
            return true;
        } else {
            pos = atomic_load_explicit(
                &ring->enqueue_pos, memory_order_relaxed);
        }
    }
}

bool cmr_try_pop(ConcurrentMpmcRing *ring, int *out_value,
                 bool *out_popped) {
    if (!ring || !out_value) return false;

    size_t pos = atomic_load_explicit(
        &ring->dequeue_pos, memory_order_relaxed);

    for (;;) {
        if (pos > SIZE_MAX - ring->capacity - 1U)
            return false;

        Cell *cell = &ring->cells[pos & ring->mask];
        size_t expected_sequence = pos + 1U;
        size_t sequence = atomic_load_explicit(
            &cell->sequence, memory_order_acquire);

        if (sequence == expected_sequence) {
            size_t expected_pos = pos;
            if (atomic_compare_exchange_weak_explicit(
                    &ring->dequeue_pos, &expected_pos, pos + 1U,
                    memory_order_relaxed, memory_order_relaxed)) {
                int value = cell->data;
                atomic_store_explicit(
                    &cell->sequence,
                    pos + ring->capacity,
                    memory_order_release);

                *out_value = value;
                if (out_popped) *out_popped = true;
                return true;
            }
            pos = expected_pos;
        } else if (sequence < expected_sequence) {
            if (out_popped) *out_popped = false;
            return true;
        } else {
            pos = atomic_load_explicit(
                &ring->dequeue_pos, memory_order_relaxed);
        }
    }
}

bool cmr_size_quiescent(const ConcurrentMpmcRing *ring,
                        size_t *out_size) {
    if (!ring || !out_size) return false;

    size_t enqueue = atomic_load_explicit(
        &ring->enqueue_pos, memory_order_acquire);
    size_t dequeue = atomic_load_explicit(
        &ring->dequeue_pos, memory_order_acquire);

    if (enqueue < dequeue ||
        enqueue - dequeue > ring->capacity)
        return false;

    *out_size = enqueue - dequeue;
    return true;
}

bool cmr_validate_quiescent(const ConcurrentMpmcRing *ring) {
    if (!ring || !ring->cells ||
        !is_power_of_two(ring->capacity) ||
        ring->mask != ring->capacity - 1U)
        return false;

    size_t size = 0;
    return cmr_size_quiescent(ring, &size) &&
           size <= ring->capacity;
}
