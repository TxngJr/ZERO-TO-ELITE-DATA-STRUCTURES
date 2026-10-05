#include "int_int_hash_table.h"
#include "hash_functions.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct Entry {
    int key;
    int value;
    struct Entry *next;
} Entry;

struct IntIntHashTable {
    Entry **buckets;
    size_t bucket_count;
    size_t size;
};

static uint64_t key_hash(int key) {
    const uint64_t bits = (uint64_t)(int64_t)key;
    return hash_u64_mix(bits);
}

static size_t bucket_for(int key, size_t bucket_count) {
    return (size_t)(key_hash(key) % bucket_count);
}

static Entry *find_entry(const IntIntHashTable *table, int key) {
    if (table == NULL || table->bucket_count == 0) return NULL;

    const size_t bucket = bucket_for(key, table->bucket_count);

    for (Entry *entry = table->buckets[bucket];
         entry != NULL;
         entry = entry->next) {
        if (entry->key == key) return entry;
    }

    return NULL;
}

static bool rehash(IntIntHashTable *table, size_t new_bucket_count) {
    if (new_bucket_count == 0 ||
        new_bucket_count > SIZE_MAX / sizeof *table->buckets) {
        return false;
    }

    Entry **new_buckets = calloc(new_bucket_count, sizeof *new_buckets);
    if (new_buckets == NULL) return false;

    for (size_t i = 0; i < table->bucket_count; ++i) {
        Entry *entry = table->buckets[i];

        while (entry != NULL) {
            Entry *next = entry->next;
            const size_t bucket = bucket_for(entry->key, new_bucket_count);

            entry->next = new_buckets[bucket];
            new_buckets[bucket] = entry;
            entry = next;
        }
    }

    free(table->buckets);
    table->buckets = new_buckets;
    table->bucket_count = new_bucket_count;
    return true;
}

static bool ensure_for_new_entry(IntIntHashTable *table) {
    if (table->size == SIZE_MAX) return false;

    const size_t next_size = table->size + 1;
    const size_t threshold = table->bucket_count - table->bucket_count / 4;

    if (next_size <= threshold) return true;

    if (table->bucket_count > SIZE_MAX / 2) return false;

    return rehash(table, table->bucket_count * 2);
}

IntIntHashTable *int_int_hash_table_create(void) {
    const size_t initial = 8;

    IntIntHashTable *table = calloc(1, sizeof *table);
    if (table == NULL) return NULL;

    table->buckets = calloc(initial, sizeof *table->buckets);
    if (table->buckets == NULL) {
        free(table);
        return NULL;
    }

    table->bucket_count = initial;
    return table;
}

void int_int_hash_table_free(IntIntHashTable *table) {
    if (table == NULL) return;

    for (size_t i = 0; i < table->bucket_count; ++i) {
        Entry *entry = table->buckets[i];

        while (entry != NULL) {
            Entry *next = entry->next;
            free(entry);
            entry = next;
        }
    }

    free(table->buckets);
    free(table);
}

size_t int_int_hash_table_size(const IntIntHashTable *table) {
    return table == NULL ? 0 : table->size;
}

size_t int_int_hash_table_bucket_count(const IntIntHashTable *table) {
    return table == NULL ? 0 : table->bucket_count;
}

double int_int_hash_table_load_factor(const IntIntHashTable *table) {
    if (table == NULL || table->bucket_count == 0) return 0.0;
    return (double)table->size / (double)table->bucket_count;
}

bool int_int_hash_table_put(IntIntHashTable *table, int key, int value) {
    if (table == NULL) return false;

    Entry *existing = find_entry(table, key);
    if (existing != NULL) {
        existing->value = value;
        return true;
    }

    if (!ensure_for_new_entry(table)) return false;

    Entry *entry = malloc(sizeof *entry);
    if (entry == NULL) return false;

    const size_t bucket = bucket_for(key, table->bucket_count);

    entry->key = key;
    entry->value = value;
    entry->next = table->buckets[bucket];
    table->buckets[bucket] = entry;
    ++table->size;
    return true;
}

bool int_int_hash_table_get(const IntIntHashTable *table, int key, int *out_value) {
    if (out_value == NULL) return false;

    Entry *entry = find_entry(table, key);
    if (entry == NULL) return false;

    *out_value = entry->value;
    return true;
}

bool int_int_hash_table_contains(const IntIntHashTable *table, int key) {
    return find_entry(table, key) != NULL;
}

bool int_int_hash_table_remove(IntIntHashTable *table, int key, int *out_value) {
    if (table == NULL) return false;

    const size_t bucket = bucket_for(key, table->bucket_count);
    Entry **link = &table->buckets[bucket];

    while (*link != NULL) {
        Entry *entry = *link;

        if (entry->key == key) {
            *link = entry->next;
            if (out_value != NULL) *out_value = entry->value;
            free(entry);
            --table->size;
            return true;
        }

        link = &entry->next;
    }

    return false;
}

bool int_int_hash_table_validate(const IntIntHashTable *table) {
    if (table == NULL || table->buckets == NULL || table->bucket_count < 8) {
        return false;
    }

    size_t count = 0;

    for (size_t i = 0; i < table->bucket_count; ++i) {
        const Entry *slow = table->buckets[i];
        const Entry *fast = table->buckets[i];

        while (fast != NULL && fast->next != NULL) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return false;
        }

        for (const Entry *entry = table->buckets[i];
             entry != NULL;
             entry = entry->next) {
            if (bucket_for(entry->key, table->bucket_count) != i) return false;
            ++count;
            if (count > table->size) return false;
        }
    }

    return count == table->size;
}
