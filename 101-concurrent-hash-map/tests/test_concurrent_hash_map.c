#include "concurrent_hash_map.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>

enum {
    THREADS = 8,
    PER = 5000
};

typedef struct {
    ConcurrentHashMap *map;
    int id;
} Arg;

static void *put_worker(void *p) {
    Arg *arg = p;
    int base = arg->id * PER;

    for (int i = 0;
         i < PER;
         ++i) {

        bool inserted = false;

        assert(chm_put(
            arg->map,
            base + i,
            (base + i) * 7,
            &inserted));

        assert(inserted);
    }

    return NULL;
}

static void *update_worker(void *p) {
    Arg *arg = p;
    int base = arg->id * PER;

    for (int i = 0;
         i < PER;
         ++i) {

        bool inserted = true;

        assert(chm_put(
            arg->map,
            base + i,
            (base + i) * 7 + 1,
            &inserted));

        assert(!inserted);
    }

    return NULL;
}

static void *remove_even_worker(void *p) {
    Arg *arg = p;
    int base = arg->id * PER;

    for (int i = 0;
         i < PER;
         i += 2) {

        bool removed = false;

        assert(chm_remove(
            arg->map,
            base + i,
            &removed));

        assert(removed);
    }

    return NULL;
}

int main(void) {
    ConcurrentHashMap *map =
        chm_create(4);

    assert(map);

    pthread_t threads[THREADS];
    Arg args[THREADS];

    for (int i = 0;
         i < THREADS;
         ++i) {

        args[i] =
            (Arg){map, i};

        assert(pthread_create(
            &threads[i],
            NULL,
            put_worker,
            &args[i]) == 0);
    }

    for (int i = 0;
         i < THREADS;
         ++i)
        pthread_join(
            threads[i],
            NULL);

    assert(
        chm_size(map) ==
        (size_t)THREADS * PER);

    size_t buckets = 0;

    assert(chm_bucket_count(
        map,
        &buckets));

    assert(buckets > 4U);
    assert(chm_resize_count(map) > 0U);
    assert(chm_validate_quiescent(map));

    for (int i = 0;
         i < THREADS;
         ++i) {

        assert(pthread_create(
            &threads[i],
            NULL,
            update_worker,
            &args[i]) == 0);
    }

    for (int i = 0;
         i < THREADS;
         ++i)
        pthread_join(
            threads[i],
            NULL);

    for (int key = 0;
         key < THREADS * PER;
         ++key) {

        int value = 0;
        bool found = false;

        assert(chm_get(
            map,
            key,
            &value,
            &found));

        assert(found);
        assert(value == key * 7 + 1);
    }

    for (int i = 0;
         i < THREADS;
         ++i) {

        assert(pthread_create(
            &threads[i],
            NULL,
            remove_even_worker,
            &args[i]) == 0);
    }

    for (int i = 0;
         i < THREADS;
         ++i)
        pthread_join(
            threads[i],
            NULL);

    assert(
        chm_size(map) ==
        (size_t)THREADS *
            PER / 2U);

    for (int key = 0;
         key < THREADS * PER;
         ++key) {

        int value = 0;
        bool found = false;

        assert(chm_get(
            map,
            key,
            &value,
            &found));

        assert(
            found ==
            (key % 2 != 0));

        if (found)
            assert(
                value ==
                key * 7 + 1);
    }

    assert(
        chm_validate_quiescent(
            map));

    printf(
        "Concurrent hash map tests passed; "
        "buckets=%zu resizes=%zu\n",
        buckets,
        chm_resize_count(map));

    chm_free(map);
    return 0;
}
