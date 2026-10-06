#include "frozen_int_set.h"
#include <stdint.h>
#include <stdlib.h>

struct FrozenIntSet {
    size_t size;
    size_t capacity;
    int *keys;
    uint8_t *used;
};

static uint64_t mix64(uint64_t x) {
    x ^= x >> 30;
    x *= UINT64_C(0xbf58476d1ce4e5b9);
    x ^= x >> 27;
    x *= UINT64_C(0x94d049bb133111eb);
    x ^= x >> 31;
    return x;
}

static size_t hash_index(int key, size_t capacity) {
    uint64_t u = (uint64_t)(int64_t)key;
    return (size_t)(mix64(u) & (uint64_t)(capacity - 1U));
}

static bool is_power_of_two(size_t x) { return x && (x & (x - 1U)) == 0; }

static size_t choose_capacity(size_t count) {
    size_t need = count < 4U ? 8U : count;
    if (need > SIZE_MAX / 2U) return 0;
    need *= 2U;
    size_t cap = 8U;
    while (cap < need) {
        if (cap > SIZE_MAX / 2U) return 0;
        cap *= 2U;
    }
    return cap;
}

FrozenIntSet *frozen_int_set_create(const int *values, size_t count) {
    if (count > 0 && !values) return NULL;
    size_t cap = choose_capacity(count);
    if (!cap || cap > SIZE_MAX / sizeof(int) || cap > SIZE_MAX / sizeof(uint8_t)) return NULL;

    FrozenIntSet *set = calloc(1, sizeof(*set));
    if (!set) return NULL;
    set->capacity = cap;
    set->keys = malloc(cap * sizeof(*set->keys));
    set->used = calloc(cap, sizeof(*set->used));
    if (!set->keys || !set->used) {
        frozen_int_set_free(set);
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        size_t pos = hash_index(values[i], cap);
        for (size_t probe = 0; probe < cap; ++probe) {
            if (!set->used[pos]) {
                set->used[pos] = 1U;
                set->keys[pos] = values[i];
                ++set->size;
                break;
            }
            if (set->keys[pos] == values[i]) break;
            pos = (pos + 1U) & (cap - 1U);
        }
    }
    return set;
}

void frozen_int_set_free(FrozenIntSet *set) {
    if (!set) return;
    free(set->keys);
    free(set->used);
    free(set);
}

size_t frozen_int_set_size(const FrozenIntSet *set) { return set ? set->size : 0; }
size_t frozen_int_set_capacity(const FrozenIntSet *set) { return set ? set->capacity : 0; }

size_t frozen_int_set_storage_bytes(const FrozenIntSet *set) {
    if (!set) return 0;
    if (set->capacity > (SIZE_MAX - sizeof(*set)) / sizeof(int)) return SIZE_MAX;
    size_t total = sizeof(*set) + set->capacity * sizeof(int);
    if (set->capacity > SIZE_MAX - total) return SIZE_MAX;
    return total + set->capacity;
}

double frozen_int_set_load_factor(const FrozenIntSet *set) {
    return (!set || set->capacity == 0) ? 0.0 : (double)set->size / (double)set->capacity;
}

bool frozen_int_set_contains(const FrozenIntSet *set, int key) {
    if (!set || !set->used || !set->keys || !is_power_of_two(set->capacity)) return false;
    size_t pos = hash_index(key, set->capacity);
    for (size_t probe = 0; probe < set->capacity; ++probe) {
        if (!set->used[pos]) return false;
        if (set->keys[pos] == key) return true;
        pos = (pos + 1U) & (set->capacity - 1U);
    }
    return false;
}

bool frozen_int_set_validate(const FrozenIntSet *set) {
    if (!set || !set->keys || !set->used || !is_power_of_two(set->capacity) || set->capacity < 8U)
        return false;
    size_t occupied = 0;
    for (size_t i = 0; i < set->capacity; ++i) {
        if (set->used[i] > 1U) return false;
        if (set->used[i]) {
            ++occupied;
            if (!frozen_int_set_contains(set, set->keys[i])) return false;
        }
    }
    if (occupied != set->size) return false;
    return set->size <= set->capacity / 2U;
}
