#include "string_int_hash_map.h"
#include "hash_functions.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    SLOT_EMPTY = 0,
    SLOT_OCCUPIED = 1,
    SLOT_TOMBSTONE = 2
} SlotState;

typedef struct {
    char *key;
    int value;
    uint64_t hash;
    SlotState state;
} Slot;

struct StringIntHashMap {
    Slot *slots;
    size_t capacity;
    size_t size;
    size_t tombstones;
};

static char *copy_string(const char *text) {
    const size_t length = strlen(text);
    if (length == SIZE_MAX) return NULL;

    char *copy = malloc(length + 1);
    if (copy == NULL) return NULL;

    memcpy(copy, text, length + 1);
    return copy;
}

static size_t next_index(size_t index, size_t capacity) {
    ++index;
    return index == capacity ? 0 : index;
}

static bool find_existing(
    const StringIntHashMap *map,
    const char *key,
    uint64_t hash,
    size_t *out_index
) {
    size_t index = (size_t)(hash % map->capacity);

    for (size_t probes = 0; probes < map->capacity; ++probes) {
        const Slot *slot = &map->slots[index];

        if (slot->state == SLOT_EMPTY) return false;

        if (slot->state == SLOT_OCCUPIED &&
            slot->hash == hash &&
            strcmp(slot->key, key) == 0) {
            if (out_index != NULL) *out_index = index;
            return true;
        }

        index = next_index(index, map->capacity);
    }

    return false;
}

static bool find_insert_slot(
    const StringIntHashMap *map,
    const char *key,
    uint64_t hash,
    size_t *out_index,
    bool *out_existing
) {
    size_t index = (size_t)(hash % map->capacity);
    size_t first_tombstone = SIZE_MAX;

    for (size_t probes = 0; probes < map->capacity; ++probes) {
        const Slot *slot = &map->slots[index];

        if (slot->state == SLOT_EMPTY) {
            *out_index = first_tombstone != SIZE_MAX ? first_tombstone : index;
            *out_existing = false;
            return true;
        }

        if (slot->state == SLOT_TOMBSTONE) {
            if (first_tombstone == SIZE_MAX) first_tombstone = index;
        } else if (slot->hash == hash && strcmp(slot->key, key) == 0) {
            *out_index = index;
            *out_existing = true;
            return true;
        }

        index = next_index(index, map->capacity);
    }

    if (first_tombstone != SIZE_MAX) {
        *out_index = first_tombstone;
        *out_existing = false;
        return true;
    }

    return false;
}

static bool rehash(StringIntHashMap *map, size_t new_capacity) {
    if (new_capacity < 16 ||
        new_capacity > SIZE_MAX / sizeof *map->slots) {
        return false;
    }

    Slot *new_slots = calloc(new_capacity, sizeof *new_slots);
    if (new_slots == NULL) return false;

    for (size_t i = 0; i < map->capacity; ++i) {
        Slot old = map->slots[i];

        if (old.state != SLOT_OCCUPIED) continue;

        size_t index = (size_t)(old.hash % new_capacity);

        while (new_slots[index].state == SLOT_OCCUPIED) {
            index = next_index(index, new_capacity);
        }

        new_slots[index] = old;
    }

    free(map->slots);
    map->slots = new_slots;
    map->capacity = new_capacity;
    map->tombstones = 0;
    return true;
}

static bool prepare_for_insert(StringIntHashMap *map) {
    if (map->size == SIZE_MAX) return false;

    const size_t next_size = map->size + 1;
    const size_t threshold = map->capacity - map->capacity / 4;

    if (next_size > threshold) {
        if (map->capacity > SIZE_MAX / 2) return false;
        return rehash(map, map->capacity * 2);
    }

    if (map->tombstones > map->size && map->tombstones > 8) {
        return rehash(map, map->capacity);
    }

    return true;
}

StringIntHashMap *string_int_hash_map_create(void) {
    const size_t initial = 16;

    StringIntHashMap *map = calloc(1, sizeof *map);
    if (map == NULL) return NULL;

    map->slots = calloc(initial, sizeof *map->slots);
    if (map->slots == NULL) {
        free(map);
        return NULL;
    }

    map->capacity = initial;
    return map;
}

void string_int_hash_map_free(StringIntHashMap *map) {
    if (map == NULL) return;

    for (size_t i = 0; i < map->capacity; ++i) {
        if (map->slots[i].state == SLOT_OCCUPIED) {
            free(map->slots[i].key);
        }
    }

    free(map->slots);
    free(map);
}

size_t string_int_hash_map_size(const StringIntHashMap *map) {
    return map == NULL ? 0 : map->size;
}

size_t string_int_hash_map_capacity(const StringIntHashMap *map) {
    return map == NULL ? 0 : map->capacity;
}

bool string_int_hash_map_put(StringIntHashMap *map, const char *key, int value) {
    if (map == NULL || key == NULL) return false;

    const uint64_t hash = hash_c_string_fnv1a(key);
    size_t index = 0;

    if (find_existing(map, key, hash, &index)) {
        map->slots[index].value = value;
        return true;
    }

    if (!prepare_for_insert(map)) return false;

    bool existing = false;
    if (!find_insert_slot(map, key, hash, &index, &existing)) return false;

    if (existing) {
        map->slots[index].value = value;
        return true;
    }

    char *owned_key = copy_string(key);
    if (owned_key == NULL) return false;

    if (map->slots[index].state == SLOT_TOMBSTONE) {
        --map->tombstones;
    }

    map->slots[index].key = owned_key;
    map->slots[index].value = value;
    map->slots[index].hash = hash;
    map->slots[index].state = SLOT_OCCUPIED;
    ++map->size;
    return true;
}

bool string_int_hash_map_get(
    const StringIntHashMap *map,
    const char *key,
    int *out_value
) {
    if (map == NULL || key == NULL || out_value == NULL) return false;

    const uint64_t hash = hash_c_string_fnv1a(key);
    size_t index = 0;

    if (!find_existing(map, key, hash, &index)) return false;

    *out_value = map->slots[index].value;
    return true;
}

bool string_int_hash_map_contains(const StringIntHashMap *map, const char *key) {
    if (map == NULL || key == NULL) return false;

    const uint64_t hash = hash_c_string_fnv1a(key);
    return find_existing(map, key, hash, NULL);
}

bool string_int_hash_map_remove(
    StringIntHashMap *map,
    const char *key,
    int *out_value
) {
    if (map == NULL || key == NULL) return false;

    const uint64_t hash = hash_c_string_fnv1a(key);
    size_t index = 0;

    if (!find_existing(map, key, hash, &index)) return false;

    Slot *slot = &map->slots[index];

    if (out_value != NULL) *out_value = slot->value;

    free(slot->key);
    slot->key = NULL;
    slot->hash = 0;
    slot->value = 0;
    slot->state = SLOT_TOMBSTONE;

    --map->size;
    ++map->tombstones;
    return true;
}

bool string_int_hash_map_validate(const StringIntHashMap *map) {
    if (map == NULL || map->slots == NULL || map->capacity < 16) return false;
    if (map->size > map->capacity) return false;
    if (map->tombstones > map->capacity - map->size) return false;

    size_t live = 0;
    size_t tombstones = 0;

    for (size_t i = 0; i < map->capacity; ++i) {
        const Slot *slot = &map->slots[i];

        if (slot->state == SLOT_EMPTY) {
            if (slot->key != NULL) return false;
        } else if (slot->state == SLOT_TOMBSTONE) {
            if (slot->key != NULL) return false;
            ++tombstones;
        } else if (slot->state == SLOT_OCCUPIED) {
            if (slot->key == NULL) return false;
            if (slot->hash != hash_c_string_fnv1a(slot->key)) return false;

            int value = 0;
            if (!string_int_hash_map_get(map, slot->key, &value)) return false;
            if (value != slot->value) return false;
            ++live;
        } else {
            return false;
        }
    }

    return live == map->size && tombstones == map->tombstones;
}
