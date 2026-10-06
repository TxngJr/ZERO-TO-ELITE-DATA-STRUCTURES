#include "concurrent_mpmc_ring.h"
#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

enum {
    PRODUCERS = 4,
    CONSUMERS = 4,
    PER = 25000,
    TOTAL = PRODUCERS * PER
};

typedef struct {
    ConcurrentMpmcRing *ring;
    int id;
} ProducerArg;

typedef struct {
    ConcurrentMpmcRing *ring;
    _Atomic unsigned char *seen;
    _Atomic size_t *count;
} ConsumerArg;

static void *producer(void *p) {
    ProducerArg *arg = p;
    int base = arg->id * PER;

    for (int i = 0; i < PER; ++i) {
        for (;;) {
            bool pushed = false;
            assert(cmr_try_push(
                arg->ring, base + i, &pushed));
            if (pushed) break;
        }
    }
    return NULL;
}

static void *consumer(void *p) {
    ConsumerArg *arg = p;

    while (atomic_load_explicit(
               arg->count, memory_order_acquire) < TOTAL) {
        int value = -1;
        bool popped = false;

        assert(cmr_try_pop(arg->ring, &value, &popped));
        if (!popped) continue;

        assert(value >= 0 && value < TOTAL);
        assert(atomic_exchange_explicit(
                   &arg->seen[value], 1U,
                   memory_order_relaxed) == 0U);
        atomic_fetch_add_explicit(
            arg->count, 1U, memory_order_release);
    }
    return NULL;
}

int main(void) {
    ConcurrentMpmcRing *ring = cmr_create(1024);
    assert(ring);
    assert(cmr_platform_lock_free(ring));
    assert(cmr_validate_quiescent(ring));

    _Atomic unsigned char *seen =
        calloc(TOTAL, sizeof(*seen));
    assert(seen);

    _Atomic size_t count;
    atomic_init(&count, 0U);

    pthread_t producer_threads[PRODUCERS];
    pthread_t consumer_threads[CONSUMERS];
    ProducerArg producer_args[PRODUCERS];
    ConsumerArg consumer_arg = {ring, seen, &count};

    for (int i = 0; i < CONSUMERS; ++i)
        assert(pthread_create(
            &consumer_threads[i], NULL,
            consumer, &consumer_arg) == 0);

    for (int i = 0; i < PRODUCERS; ++i) {
        producer_args[i] = (ProducerArg){ring, i};
        assert(pthread_create(
            &producer_threads[i], NULL,
            producer, &producer_args[i]) == 0);
    }

    for (int i = 0; i < PRODUCERS; ++i)
        pthread_join(producer_threads[i], NULL);

    for (int i = 0; i < CONSUMERS; ++i)
        pthread_join(consumer_threads[i], NULL);

    assert(atomic_load(&count) == TOTAL);

    for (int i = 0; i < TOTAL; ++i)
        assert(atomic_load(&seen[i]) == 1U);

    size_t size = 1U;
    assert(cmr_size_quiescent(ring, &size));
    assert(size == 0U);
    assert(cmr_validate_quiescent(ring));

    free(seen);
    cmr_free(ring);

    puts("Concurrent MPMC ring tests passed");
    return 0;
}
