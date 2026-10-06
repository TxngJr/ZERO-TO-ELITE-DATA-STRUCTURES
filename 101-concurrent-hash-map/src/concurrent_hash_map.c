#define _POSIX_C_SOURCE 200809L
#include "concurrent_hash_map.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct MapNode {
    int key;
    int value;
    struct MapNode *next;
} MapNode;

typedef struct {
    pthread_mutex_t mu;
    MapNode *head;
} Bucket;

struct ConcurrentHashMap {
    pthread_rwlock_t table_lock;
    Bucket *buckets;
    size_t bucket_count;
    _Atomic size_t size;
    _Atomic size_t resize_count;
};

static uint64_t mix64(uint64_t x) {
    x ^= x >> 30;
    x *= UINT64_C(0xbf58476d1ce4e5b9);
    x ^= x >> 27;
    x *= UINT64_C(0x94d049bb133111eb);
    x ^= x >> 31;
    return x;
}

static size_t index_for(int key, size_t bucket_count) {
    return (size_t)(
        mix64((uint64_t)(int64_t)key) &
        (uint64_t)(bucket_count - 1U));
}

static bool next_power_two(size_t requested, size_t *out) {
    size_t n = 4U;
    while (n < requested) {
        if (n > SIZE_MAX / 2U) return false;
        n *= 2U;
    }
    *out = n;
    return true;
}

static Bucket *bucket_array_create(size_t count) {
    if (count > SIZE_MAX / sizeof(Bucket)) return NULL;

    Bucket *buckets = calloc(count, sizeof(*buckets));
    if (!buckets) return NULL;

    size_t i = 0;
    for (; i < count; ++i) {
        if (pthread_mutex_init(&buckets[i].mu, NULL) != 0)
            break;
    }

    if (i != count) {
        while (i > 0)
            pthread_mutex_destroy(&buckets[--i].mu);
        free(buckets);
        return NULL;
    }
    return buckets;
}

static void bucket_array_destroy(Bucket *buckets,
                                 size_t count,
                                 bool free_nodes) {
    if (!buckets) return;

    for (size_t i = 0; i < count; ++i) {
        if (free_nodes) {
            MapNode *node = buckets[i].head;
            while (node) {
                MapNode *next = node->next;
                free(node);
                node = next;
            }
        }
        pthread_mutex_destroy(&buckets[i].mu);
    }
    free(buckets);
}

ConcurrentHashMap *chm_create(size_t initial_buckets) {
    size_t count = 0;
    if (!next_power_two(
            initial_buckets ? initial_buckets : 4U,
            &count))
        return NULL;

    ConcurrentHashMap *map = calloc(1, sizeof(*map));
    if (!map) return NULL;

    if (pthread_rwlock_init(&map->table_lock, NULL) != 0) {
        free(map);
        return NULL;
    }

    map->buckets = bucket_array_create(count);
    if (!map->buckets) {
        pthread_rwlock_destroy(&map->table_lock);
        free(map);
        return NULL;
    }

    map->bucket_count = count;
    atomic_init(&map->size, 0U);
    atomic_init(&map->resize_count, 0U);
    return map;
}

void chm_free(ConcurrentHashMap *map) {
    if (!map) return;
    bucket_array_destroy(
        map->buckets, map->bucket_count, true);
    pthread_rwlock_destroy(&map->table_lock);
    free(map);
}

static void maybe_resize(ConcurrentHashMap *map) {
    size_t size =
        atomic_load_explicit(
            &map->size, memory_order_relaxed);

    size_t observed_buckets = 0;
    if (pthread_rwlock_rdlock(
            &map->table_lock) != 0)
        return;

    observed_buckets = map->bucket_count;
    pthread_rwlock_unlock(&map->table_lock);

    if (observed_buckets > SIZE_MAX / 4U ||
        size <= observed_buckets * 4U)
        return;

    if (pthread_rwlock_wrlock(
            &map->table_lock) != 0)
        return;

    size = atomic_load_explicit(
        &map->size, memory_order_relaxed);
    size_t old_count = map->bucket_count;

    if (old_count <= SIZE_MAX / 4U &&
        size > old_count * 4U &&
        old_count <= SIZE_MAX / 2U) {

        size_t new_count = old_count * 2U;
        Bucket *new_buckets =
            bucket_array_create(new_count);

        if (new_buckets) {
            for (size_t i = 0;
                 i < old_count;
                 ++i) {

                MapNode *node =
                    map->buckets[i].head;

                while (node) {
                    MapNode *next = node->next;
                    size_t j =
                        index_for(
                            node->key,
                            new_count);

                    node->next =
                        new_buckets[j].head;
                    new_buckets[j].head =
                        node;
                    node = next;
                }

                map->buckets[i].head =
                    NULL;
            }

            Bucket *old_buckets =
                map->buckets;

            map->buckets =
                new_buckets;
            map->bucket_count =
                new_count;

            bucket_array_destroy(
                old_buckets,
                old_count,
                false);

            atomic_fetch_add_explicit(
                &map->resize_count,
                1U,
                memory_order_relaxed);
        }
    }

    pthread_rwlock_unlock(
        &map->table_lock);
}

bool chm_put(ConcurrentHashMap *map,
             int key,
             int value,
             bool *out_inserted) {
    if (!map) return false;

    if (pthread_rwlock_rdlock(
            &map->table_lock) != 0)
        return false;

    size_t idx =
        index_for(
            key,
            map->bucket_count);

    Bucket *bucket =
        &map->buckets[idx];

    if (pthread_mutex_lock(
            &bucket->mu) != 0) {
        pthread_rwlock_unlock(
            &map->table_lock);
        return false;
    }

    for (MapNode *node = bucket->head;
         node;
         node = node->next) {

        if (node->key == key) {
            node->value = value;

            int unlock_bucket =
                pthread_mutex_unlock(
                    &bucket->mu);

            int unlock_table =
                pthread_rwlock_unlock(
                    &map->table_lock);

            if (unlock_bucket ||
                unlock_table)
                return false;

            if (out_inserted)
                *out_inserted = false;
            return true;
        }
    }

    MapNode *node =
        malloc(sizeof(*node));

    if (!node) {
        pthread_mutex_unlock(
            &bucket->mu);
        pthread_rwlock_unlock(
            &map->table_lock);
        return false;
    }

    node->key = key;
    node->value = value;
    node->next = bucket->head;
    bucket->head = node;

    atomic_fetch_add_explicit(
        &map->size,
        1U,
        memory_order_relaxed);

    int unlock_bucket =
        pthread_mutex_unlock(
            &bucket->mu);

    int unlock_table =
        pthread_rwlock_unlock(
            &map->table_lock);

    if (unlock_bucket ||
        unlock_table)
        return false;

    if (out_inserted)
        *out_inserted = true;

    maybe_resize(map);
    return true;
}

bool chm_get(ConcurrentHashMap *map,
             int key,
             int *out_value,
             bool *out_found) {
    if (!map ||
        !out_value ||
        !out_found)
        return false;

    if (pthread_rwlock_rdlock(
            &map->table_lock) != 0)
        return false;

    Bucket *bucket =
        &map->buckets[
            index_for(
                key,
                map->bucket_count)];

    if (pthread_mutex_lock(
            &bucket->mu) != 0) {
        pthread_rwlock_unlock(
            &map->table_lock);
        return false;
    }

    bool found = false;
    int value = 0;

    for (MapNode *node = bucket->head;
         node;
         node = node->next) {

        if (node->key == key) {
            found = true;
            value = node->value;
            break;
        }
    }

    int unlock_bucket =
        pthread_mutex_unlock(
            &bucket->mu);

    int unlock_table =
        pthread_rwlock_unlock(
            &map->table_lock);

    if (unlock_bucket ||
        unlock_table)
        return false;

    *out_found = found;
    if (found)
        *out_value = value;

    return true;
}

bool chm_remove(ConcurrentHashMap *map,
                int key,
                bool *out_removed) {
    if (!map) return false;

    if (pthread_rwlock_rdlock(
            &map->table_lock) != 0)
        return false;

    Bucket *bucket =
        &map->buckets[
            index_for(
                key,
                map->bucket_count)];

    if (pthread_mutex_lock(
            &bucket->mu) != 0) {
        pthread_rwlock_unlock(
            &map->table_lock);
        return false;
    }

    MapNode **link =
        &bucket->head;

    while (*link &&
           (*link)->key != key)
        link = &(*link)->next;

    MapNode *dead = NULL;
    bool removed = false;

    if (*link) {
        dead = *link;
        *link = dead->next;

        atomic_fetch_sub_explicit(
            &map->size,
            1U,
            memory_order_relaxed);

        removed = true;
    }

    int unlock_bucket =
        pthread_mutex_unlock(
            &bucket->mu);

    int unlock_table =
        pthread_rwlock_unlock(
            &map->table_lock);

    if (unlock_bucket ||
        unlock_table) {
        free(dead);
        return false;
    }

    free(dead);

    if (out_removed)
        *out_removed = removed;

    return true;
}

size_t chm_size(
    const ConcurrentHashMap *map) {
    return map
        ? atomic_load_explicit(
              &map->size,
              memory_order_relaxed)
        : 0U;
}

size_t chm_resize_count(
    const ConcurrentHashMap *map) {
    return map
        ? atomic_load_explicit(
              &map->resize_count,
              memory_order_relaxed)
        : 0U;
}

bool chm_bucket_count(
    ConcurrentHashMap *map,
    size_t *out_count) {
    if (!map || !out_count)
        return false;

    if (pthread_rwlock_rdlock(
            &map->table_lock) != 0)
        return false;

    *out_count =
        map->bucket_count;

    return pthread_rwlock_unlock(
               &map->table_lock) == 0;
}

bool chm_validate_quiescent(
    ConcurrentHashMap *map) {
    if (!map) return false;

    if (pthread_rwlock_wrlock(
            &map->table_lock) != 0)
        return false;

    bool ok =
        map->buckets &&
        map->bucket_count >= 4U &&
        (map->bucket_count &
         (map->bucket_count - 1U)) == 0U;

    size_t count = 0;

    for (size_t i = 0;
         ok && i < map->bucket_count;
         ++i) {

        for (MapNode *a =
                 map->buckets[i].head;
             a;
             a = a->next) {

            ++count;

            if (index_for(
                    a->key,
                    map->bucket_count) != i) {
                ok = false;
                break;
            }

            for (MapNode *b = a->next;
                 b;
                 b = b->next) {

                if (a->key == b->key) {
                    ok = false;
                    break;
                }
            }
        }
    }

    if (ok) {
        ok =
            count ==
            atomic_load_explicit(
                &map->size,
                memory_order_relaxed);
    }

    if (pthread_rwlock_unlock(
            &map->table_lock) != 0)
        return false;

    return ok;
}
