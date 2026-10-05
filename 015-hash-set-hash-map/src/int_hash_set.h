#ifndef INT_HASH_SET_H
#define INT_HASH_SET_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntHashSet IntHashSet;

IntHashSet *int_hash_set_create(void);
void int_hash_set_free(IntHashSet *set);
size_t int_hash_set_size(const IntHashSet *set);
bool int_hash_set_add(IntHashSet *set, int value);
bool int_hash_set_contains(const IntHashSet *set, int value);
bool int_hash_set_remove(IntHashSet *set, int value);
bool int_hash_set_validate(const IntHashSet *set);

#endif
