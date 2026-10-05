#ifndef U64_CUCKOO_SET_H
#define U64_CUCKOO_SET_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct U64CuckooSet U64CuckooSet;

U64CuckooSet *u64_cuckoo_set_create(size_t table_capacity);
void u64_cuckoo_set_free(U64CuckooSet *set);
size_t u64_cuckoo_set_size(const U64CuckooSet *set);
size_t u64_cuckoo_set_table_capacity(const U64CuckooSet *set);
bool u64_cuckoo_set_insert(U64CuckooSet *set,uint64_t key);
bool u64_cuckoo_set_remove(U64CuckooSet *set,uint64_t key);
bool u64_cuckoo_set_contains(const U64CuckooSet *set,uint64_t key);
bool u64_cuckoo_set_validate(const U64CuckooSet *set);

#endif
