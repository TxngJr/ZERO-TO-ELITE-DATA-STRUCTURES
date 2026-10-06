#include "atomic_structures.h"
#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

enum {
    THREADS = 8,
    BITS = 8192,
    INCREMENTS = 10000
};

typedef struct {
    AtomicBitset *set;
    int id;
} BitArg;

typedef struct {
    AtomicTaggedValue *value;
    _Atomic size_t *retries;
} IncrementArg;

static void *set_worker(void *p) {
    BitArg *arg = p;
    for (size_t i = (size_t)arg->id; i < BITS; i += THREADS) {
        bool previous = true;
        assert(abs_test_and_set(arg->set, i, &previous));
        assert(!previous);
    }
    return NULL;
}

static void *increment_worker(void *p) {
    IncrementArg *arg = p;
    for (int i = 0; i < INCREMENTS; ++i) {
        size_t retries = 0;
        assert(atv_fetch_increment(arg->value, NULL, &retries));
        atomic_fetch_add_explicit(
            arg->retries, retries, memory_order_relaxed);
    }
    return NULL;
}

int main(void) {
    AtomicBitset *set = abs_create(BITS + 3U);
    assert(set);
    assert(abs_platform_lock_free(set));
    assert(abs_validate_quiescent(set));

    pthread_t threads[THREADS];
    BitArg bit_args[THREADS];

    for (int i = 0; i < THREADS; ++i) {
        bit_args[i] = (BitArg){set, i};
        assert(pthread_create(
            &threads[i], NULL, set_worker, &bit_args[i]) == 0);
    }
    for (int i = 0; i < THREADS; ++i)
        pthread_join(threads[i], NULL);

    assert(abs_count_quiescent(set) == BITS);

    for (size_t i = 0; i < BITS; ++i) {
        bool value = false;
        assert(abs_test(set, i, &value));
        assert(value);
    }

    bool previous = false;
    assert(abs_test_and_toggle(set, BITS + 1U, &previous));
    assert(!previous);
    assert(abs_test_and_clear(set, BITS + 1U, &previous));
    assert(previous);

    size_t last_word = (BITS + 3U + 63U) / 64U - 1U;
    uint64_t expected = 0U;
    bool swapped = false;
    assert(!abs_compare_exchange_word(
        set, last_word, &expected, UINT64_MAX, &swapped));

    assert(abs_validate_quiescent(set));
    abs_free(set);

    AtomicTaggedValue *tagged = atv_create(7U);
    assert(tagged);
    assert(atv_platform_lock_free(tagged));

    ATVSnapshot old = {0};
    assert(atv_load(tagged, &old));

    ATVSnapshot expected_snapshot = old;
    assert(atv_compare_exchange(
        tagged, &expected_snapshot, 9U, &swapped));
    assert(swapped);

    assert(atv_load(tagged, &expected_snapshot));
    assert(atv_compare_exchange(
        tagged, &expected_snapshot, 7U, &swapped));
    assert(swapped);

    ATVSnapshot stale = old;
    assert(atv_compare_exchange(tagged, &stale, 10U, &swapped));
    assert(!swapped);
    assert(stale.value == 7U);
    assert(stale.version == 2U);
    atv_free(tagged);

    AtomicTaggedValue *counter = atv_create(0U);
    assert(counter);
    assert(atv_platform_lock_free(counter));

    _Atomic size_t retries;
    atomic_init(&retries, 0U);
    IncrementArg increment_arg = {counter, &retries};

    for (int i = 0; i < THREADS; ++i)
        assert(pthread_create(
            &threads[i], NULL, increment_worker, &increment_arg) == 0);

    for (int i = 0; i < THREADS; ++i)
        pthread_join(threads[i], NULL);

    ATVSnapshot final = {0};
    assert(atv_load(counter, &final));
    assert(final.value == THREADS * INCREMENTS);
    assert(final.version == THREADS * INCREMENTS);

    printf("Atomic structures tests passed; CAS retries=%zu\n",
           atomic_load(&retries));

    atv_free(counter);
    return 0;
}
