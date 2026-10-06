#include "object_pool.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdalign.h>

struct ObjectPool {
    size_t object_size;
    size_t stride;
    size_t capacity;
    size_t free_top;
    size_t in_use_count;
    unsigned char *storage;
    size_t *free_stack;
    unsigned char *in_use;
};

static bool round_up(size_t value, size_t alignment, size_t *out) {
    if (alignment == 0U) return false;
    size_t rem = value % alignment;
    if (rem == 0U) {
        *out = value;
        return true;
    }
    size_t add = alignment - rem;
    if (value > SIZE_MAX - add) return false;
    *out = value + add;
    return true;
}

ObjectPool *op_create(size_t object_size, size_t capacity) {
    if (object_size == 0U || capacity == 0U) return NULL;

    size_t stride = 0U;
    if (!round_up(object_size, alignof(max_align_t), &stride)) return NULL;
    if (stride > SIZE_MAX / capacity) return NULL;
    if (capacity > SIZE_MAX / sizeof(size_t)) return NULL;

    ObjectPool *pool = calloc(1, sizeof(*pool));
    if (!pool) return NULL;

    pool->storage =
        aligned_alloc(alignof(max_align_t), stride * capacity);
    pool->free_stack =
        malloc(capacity * sizeof(*pool->free_stack));
    pool->in_use =
        calloc(capacity, sizeof(*pool->in_use));

    if (!pool->storage || !pool->free_stack || !pool->in_use) {
        free(pool->storage);
        free(pool->free_stack);
        free(pool->in_use);
        free(pool);
        return NULL;
    }

    pool->object_size = object_size;
    pool->stride = stride;
    pool->capacity = capacity;
    pool->free_top = capacity;

    for (size_t i = 0; i < capacity; ++i)
        pool->free_stack[i] = capacity - 1U - i;

    return pool;
}

void op_free(ObjectPool *pool) {
    if (!pool) return;
    free(pool->storage);
    free(pool->free_stack);
    free(pool->in_use);
    free(pool);
}

void *op_alloc(ObjectPool *pool) {
    if (!pool || pool->free_top == 0U) return NULL;

    size_t idx = pool->free_stack[--pool->free_top];
    if (idx >= pool->capacity || pool->in_use[idx]) {
        ++pool->free_top;
        return NULL;
    }

    pool->in_use[idx] = 1U;
    ++pool->in_use_count;

    unsigned char *ptr =
        pool->storage + idx * pool->stride;
    memset(ptr, 0, pool->object_size);
    return ptr;
}

static bool pointer_index(const ObjectPool *pool,
                          const void *ptr,
                          size_t *out_idx) {
    if (!pool || !ptr || !pool->storage) return false;

    uintptr_t base = (uintptr_t)pool->storage;
    uintptr_t p = (uintptr_t)ptr;
    if (p < base) return false;

    uintptr_t delta = p - base;
    if (delta >= pool->stride * pool->capacity) return false;
    if (delta % pool->stride != 0U) return false;

    size_t idx = (size_t)(delta / pool->stride);
    if (idx >= pool->capacity) return false;
    if (out_idx) *out_idx = idx;
    return true;
}

bool op_release(ObjectPool *pool, void *ptr) {
    size_t idx = 0U;
    if (!pointer_index(pool, ptr, &idx)) return false;
    if (!pool->in_use[idx]) return false;
    if (pool->free_top >= pool->capacity) return false;

    pool->in_use[idx] = 0U;
    --pool->in_use_count;
    pool->free_stack[pool->free_top++] = idx;
    return true;
}

bool op_owns(const ObjectPool *pool, const void *ptr) {
    return pointer_index(pool, ptr, NULL);
}

size_t op_capacity(const ObjectPool *pool) {
    return pool ? pool->capacity : 0U;
}
size_t op_object_size(const ObjectPool *pool) {
    return pool ? pool->object_size : 0U;
}
size_t op_stride(const ObjectPool *pool) {
    return pool ? pool->stride : 0U;
}
size_t op_in_use_count(const ObjectPool *pool) {
    return pool ? pool->in_use_count : 0U;
}

bool op_validate(const ObjectPool *pool) {
    if (!pool || !pool->storage || !pool->free_stack ||
        !pool->in_use)
        return false;

    if (pool->object_size == 0U ||
        pool->stride < pool->object_size ||
        pool->capacity == 0U)
        return false;

    if (pool->free_top > pool->capacity ||
        pool->in_use_count > pool->capacity)
        return false;

    if (pool->free_top + pool->in_use_count !=
        pool->capacity)
        return false;

    unsigned char *seen =
        calloc(pool->capacity, 1U);
    if (!seen) return false;

    size_t live = 0U;
    bool ok = true;

    for (size_t i = 0; i < pool->capacity; ++i)
        if (pool->in_use[i]) ++live;

    if (live != pool->in_use_count) ok = false;

    for (size_t i = 0; ok && i < pool->free_top; ++i) {
        size_t idx = pool->free_stack[i];
        if (idx >= pool->capacity ||
            seen[idx] ||
            pool->in_use[idx]) {
            ok = false;
        } else {
            seen[idx] = 1U;
        }
    }

    free(seen);
    return ok;
}
