#include "cow_int_vector.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t refs;
    size_t size;
    size_t capacity;
    int data[];
} CowStorage;

struct CowIntVector {
    CowStorage *storage;
};

static CowStorage *storage_create(size_t capacity, size_t size, const int *src) {
    if (size > capacity || capacity > (SIZE_MAX - sizeof(CowStorage)) / sizeof(int)) return NULL;
    CowStorage *s = malloc(sizeof(*s) + capacity * sizeof(int));
    if (!s) return NULL;
    s->refs = 1U;
    s->size = size;
    s->capacity = capacity;
    if (size && src) memcpy(s->data, src, size * sizeof(int));
    return s;
}

static bool next_capacity(size_t current, size_t need, size_t *out) {
    size_t cap = current ? current : 4U;
    while (cap < need) {
        if (cap > SIZE_MAX / 2U) return false;
        cap *= 2U;
    }
    *out = cap;
    return true;
}

static bool detach(CowIntVector *v, size_t min_capacity) {
    if (!v || !v->storage) return false;
    CowStorage *old = v->storage;
    size_t cap = old->capacity;
    if (cap < min_capacity && !next_capacity(cap, min_capacity, &cap)) return false;
    if (old->refs == 1U && cap == old->capacity) return true;

    CowStorage *copy = storage_create(cap, old->size, old->data);
    if (!copy) return false;

    --old->refs;
    if (old->refs == 0U) free(old);
    v->storage = copy;
    return true;
}

CowIntVector *cow_vector_create(void) {
    CowIntVector *v = malloc(sizeof(*v));
    if (!v) return NULL;
    v->storage = storage_create(4U, 0U, NULL);
    if (!v->storage) {
        free(v);
        return NULL;
    }
    return v;
}

CowIntVector *cow_vector_from_array(const int *values, size_t count) {
    if (count && !values) return NULL;
    size_t cap = 4U;
    if (!next_capacity(cap, count, &cap)) return NULL;

    CowIntVector *v = malloc(sizeof(*v));
    if (!v) return NULL;
    v->storage = storage_create(cap, count, values);
    if (!v->storage) {
        free(v);
        return NULL;
    }
    return v;
}

CowIntVector *cow_vector_clone(const CowIntVector *src) {
    if (!src || !src->storage || src->storage->refs == SIZE_MAX) return NULL;
    CowIntVector *v = malloc(sizeof(*v));
    if (!v) return NULL;
    ++src->storage->refs;
    v->storage = src->storage;
    return v;
}

void cow_vector_free(CowIntVector *v) {
    if (!v) return;
    if (v->storage && --v->storage->refs == 0U) free(v->storage);
    free(v);
}

size_t cow_vector_size(const CowIntVector *v) {
    return v && v->storage ? v->storage->size : 0U;
}

size_t cow_vector_capacity(const CowIntVector *v) {
    return v && v->storage ? v->storage->capacity : 0U;
}

size_t cow_vector_share_count(const CowIntVector *v) {
    return v && v->storage ? v->storage->refs : 0U;
}

bool cow_vector_get(const CowIntVector *v, size_t index, int *out) {
    if (!v || !v->storage || !out || index >= v->storage->size) return false;
    *out = v->storage->data[index];
    return true;
}

bool cow_vector_set(CowIntVector *v, size_t index, int value) {
    if (!v || !v->storage || index >= v->storage->size) return false;
    if (!detach(v, v->storage->capacity)) return false;
    v->storage->data[index] = value;
    return true;
}

bool cow_vector_push(CowIntVector *v, int value) {
    if (!v || !v->storage || v->storage->size == SIZE_MAX) return false;
    size_t need = v->storage->size + 1U;
    if (!detach(v, need)) return false;
    v->storage->data[v->storage->size++] = value;
    return true;
}

bool cow_vector_pop(CowIntVector *v, int *out) {
    if (!v || !v->storage || v->storage->size == 0U) return false;
    if (!detach(v, v->storage->capacity)) return false;
    size_t idx = v->storage->size - 1U;
    if (out) *out = v->storage->data[idx];
    v->storage->size = idx;
    return true;
}

bool cow_vector_validate(const CowIntVector *v) {
    return v && v->storage && v->storage->refs > 0U &&
           v->storage->size <= v->storage->capacity &&
           v->storage->capacity >= 4U;
}
