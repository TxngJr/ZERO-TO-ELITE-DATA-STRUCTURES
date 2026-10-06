#include "concurrent_int_set.h"
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct ConcurrentIntSet {
    pthread_mutex_t mu;
    int *data;
    size_t size;
    size_t capacity;
};

static size_t lower_bound(const int *a, size_t n, int x) {
    size_t l = 0, h = n;
    while (l < h) {
        size_t m = l + (h - l) / 2U;
        if (a[m] < x) l = m + 1U;
        else h = m;
    }
    return l;
}

ConcurrentIntSet *cis_create(void) {
    ConcurrentIntSet *s = calloc(1, sizeof(*s));
    if (!s) return NULL;
    if (pthread_mutex_init(&s->mu, NULL) != 0) {
        free(s);
        return NULL;
    }

    s->capacity = 8U;
    s->data = malloc(s->capacity * sizeof(int));
    if (!s->data) {
        pthread_mutex_destroy(&s->mu);
        free(s);
        return NULL;
    }
    return s;
}

void cis_free(ConcurrentIntSet *s) {
    if (!s) return;
    pthread_mutex_destroy(&s->mu);
    free(s->data);
    free(s);
}

static bool grow_locked(ConcurrentIntSet *s) {
    if (s->capacity > SIZE_MAX / 2U) return false;
    size_t cap = s->capacity * 2U;
    if (cap > SIZE_MAX / sizeof(int)) return false;

    int *p = realloc(s->data, cap * sizeof(int));
    if (!p) return false;
    s->data = p;
    s->capacity = cap;
    return true;
}

bool cis_insert(ConcurrentIntSet *s, int value, bool *out_inserted) {
    if (!s) return false;
    if (pthread_mutex_lock(&s->mu) != 0) return false;

    bool ok = true, inserted = false;
    size_t pos = lower_bound(s->data, s->size, value);

    if (!(pos < s->size && s->data[pos] == value)) {
        if (s->size == s->capacity && !grow_locked(s)) {
            ok = false;
        } else {
            memmove(&s->data[pos + 1U], &s->data[pos],
                    (s->size - pos) * sizeof(int));
            s->data[pos] = value;
            ++s->size;
            inserted = true;
        }
    }

    if (pthread_mutex_unlock(&s->mu) != 0) return false;
    if (ok && out_inserted) *out_inserted = inserted;
    return ok;
}

bool cis_remove(ConcurrentIntSet *s, int value, bool *out_removed) {
    if (!s) return false;
    if (pthread_mutex_lock(&s->mu) != 0) return false;

    bool removed = false;
    size_t pos = lower_bound(s->data, s->size, value);
    if (pos < s->size && s->data[pos] == value) {
        memmove(&s->data[pos], &s->data[pos + 1U],
                (s->size - pos - 1U) * sizeof(int));
        --s->size;
        removed = true;
    }

    if (pthread_mutex_unlock(&s->mu) != 0) return false;
    if (out_removed) *out_removed = removed;
    return true;
}

bool cis_contains(ConcurrentIntSet *s, int value, bool *out_contains) {
    if (!s || !out_contains) return false;
    if (pthread_mutex_lock(&s->mu) != 0) return false;

    size_t pos = lower_bound(s->data, s->size, value);
    *out_contains = pos < s->size && s->data[pos] == value;
    return pthread_mutex_unlock(&s->mu) == 0;
}

bool cis_size(ConcurrentIntSet *s, size_t *out_size) {
    if (!s || !out_size) return false;
    if (pthread_mutex_lock(&s->mu) != 0) return false;

    *out_size = s->size;
    return pthread_mutex_unlock(&s->mu) == 0;
}

bool cis_snapshot(ConcurrentIntSet *s, int **out_values, size_t *out_count) {
    if (!s || !out_values || !out_count) return false;
    if (pthread_mutex_lock(&s->mu) != 0) return false;

    int *copy = NULL;
    bool ok = true;
    if (s->size) {
        if (s->size > SIZE_MAX / sizeof(int)) {
            ok = false;
        } else {
            copy = malloc(s->size * sizeof(int));
            if (!copy) ok = false;
            else memcpy(copy, s->data, s->size * sizeof(int));
        }
    }
    size_t n = s->size;

    if (pthread_mutex_unlock(&s->mu) != 0) {
        free(copy);
        return false;
    }
    if (!ok) {
        free(copy);
        return false;
    }

    *out_values = copy;
    *out_count = n;
    return true;
}

bool cis_validate(ConcurrentIntSet *s) {
    if (!s) return false;
    if (pthread_mutex_lock(&s->mu) != 0) return false;

    bool ok = s->data && s->capacity >= 8U && s->size <= s->capacity;
    for (size_t i = 1; ok && i < s->size; ++i) {
        if (s->data[i - 1U] >= s->data[i]) ok = false;
    }

    if (pthread_mutex_unlock(&s->mu) != 0) return false;
    return ok;
}
