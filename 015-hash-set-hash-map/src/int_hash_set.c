#include "int_hash_set.h"
#include "int_int_hash_table.h"

#include <stdlib.h>

struct IntHashSet {
    IntIntHashTable *table;
};

IntHashSet *int_hash_set_create(void) {
    IntHashSet *set = malloc(sizeof *set);
    if (set == NULL) return NULL;

    set->table = int_int_hash_table_create();
    if (set->table == NULL) {
        free(set);
        return NULL;
    }

    return set;
}

void int_hash_set_free(IntHashSet *set) {
    if (set == NULL) return;
    int_int_hash_table_free(set->table);
    free(set);
}

size_t int_hash_set_size(const IntHashSet *set) {
    return set == NULL ? 0 : int_int_hash_table_size(set->table);
}

bool int_hash_set_add(IntHashSet *set, int value) {
    if (set == NULL) return false;
    return int_int_hash_table_put(set->table, value, 1);
}

bool int_hash_set_contains(const IntHashSet *set, int value) {
    return set != NULL && int_int_hash_table_contains(set->table, value);
}

bool int_hash_set_remove(IntHashSet *set, int value) {
    return set != NULL && int_int_hash_table_remove(set->table, value, NULL);
}

bool int_hash_set_validate(const IntHashSet *set) {
    return set != NULL && int_int_hash_table_validate(set->table);
}
