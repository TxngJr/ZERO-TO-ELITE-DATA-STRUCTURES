#ifndef FROZEN_INT_SET_H
#define FROZEN_INT_SET_H
#include <stdbool.h>
#include <stddef.h>

typedef struct FrozenIntSet FrozenIntSet;

/* Build-once set. Input is copied; duplicate keys are coalesced. */
FrozenIntSet *frozen_int_set_create(const int *values, size_t count);
void frozen_int_set_free(FrozenIntSet *set);

size_t frozen_int_set_size(const FrozenIntSet *set);
size_t frozen_int_set_capacity(const FrozenIntSet *set);
size_t frozen_int_set_storage_bytes(const FrozenIntSet *set);
double frozen_int_set_load_factor(const FrozenIntSet *set);
bool frozen_int_set_contains(const FrozenIntSet *set, int key);
bool frozen_int_set_validate(const FrozenIntSet *set);
#endif
