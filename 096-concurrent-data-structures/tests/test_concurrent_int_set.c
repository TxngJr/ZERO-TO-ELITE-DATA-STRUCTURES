#include "concurrent_int_set.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

enum { THREADS = 8, PER_THREAD = 4000 };

typedef struct {
    ConcurrentIntSet *set;
    int id;
} Arg;

static void *insert_worker(void *p) {
    Arg *a = p;
    int base = a->id * PER_THREAD;
    for (int i = 0; i < PER_THREAD; ++i) {
        bool inserted = false;
        assert(cis_insert(a->set, base + i, &inserted));
        assert(inserted);
    }
    return NULL;
}

static void *remove_even_worker(void *p) {
    Arg *a = p;
    int base = a->id * PER_THREAD;
    for (int i = 0; i < PER_THREAD; i += 2) {
        bool removed = false;
        assert(cis_remove(a->set, base + i, &removed));
        assert(removed);
    }
    return NULL;
}

static void *duplicate_worker(void *p) {
    ConcurrentIntSet *set = p;
    for (int round = 0; round < 10; ++round) {
        for (int i = 1; i < THREADS * PER_THREAD; i += 2) {
            bool inserted = false;
            assert(cis_insert(set, i, &inserted));
        }
    }
    return NULL;
}

int main(void) {
    ConcurrentIntSet *set = cis_create();
    assert(set && cis_validate(set));

    pthread_t threads[THREADS];
    Arg args[THREADS];

    for (int t = 0; t < THREADS; ++t) {
        args[t] = (Arg){set, t};
        assert(pthread_create(&threads[t], NULL, insert_worker, &args[t]) == 0);
    }
    for (int t = 0; t < THREADS; ++t)
        assert(pthread_join(threads[t], NULL) == 0);

    size_t n = 0;
    assert(cis_size(set, &n) && n == (size_t)THREADS * PER_THREAD);
    assert(cis_validate(set));

    for (int t = 0; t < THREADS; ++t)
        assert(pthread_create(&threads[t], NULL, remove_even_worker, &args[t]) == 0);
    for (int t = 0; t < THREADS; ++t)
        assert(pthread_join(threads[t], NULL) == 0);

    assert(cis_size(set, &n) && n == (size_t)THREADS * PER_THREAD / 2U);
    assert(cis_validate(set));

    pthread_t duplicate_threads[4];
    for (int t = 0; t < 4; ++t)
        assert(pthread_create(&duplicate_threads[t], NULL, duplicate_worker, set) == 0);
    for (int t = 0; t < 4; ++t)
        assert(pthread_join(duplicate_threads[t], NULL) == 0);

    assert(cis_size(set, &n) && n == (size_t)THREADS * PER_THREAD / 2U);

    for (int i = 0; i < THREADS * PER_THREAD; ++i) {
        bool has = false;
        assert(cis_contains(set, i, &has));
        assert(has == (i % 2 != 0));
    }

    int *snapshot = NULL;
    size_t count = 0;
    assert(cis_snapshot(set, &snapshot, &count) && count == n);
    for (size_t i = 1; i < count; ++i)
        assert(snapshot[i - 1U] < snapshot[i]);
    free(snapshot);

    puts("Concurrent set tests passed");
    cis_free(set);
    return 0;
}
