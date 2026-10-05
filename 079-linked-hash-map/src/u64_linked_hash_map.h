#ifndef U64_LINKED_HASH_MAP_H
#define U64_LINKED_HASH_MAP_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct U64LinkedHashMap U64LinkedHashMap;
U64LinkedHashMap *u64_lhm_create(size_t bucket_count);
void u64_lhm_free(U64LinkedHashMap *map);
size_t u64_lhm_size(const U64LinkedHashMap *map);
bool u64_lhm_put(U64LinkedHashMap *map,uint64_t key,int64_t value);
bool u64_lhm_get(const U64LinkedHashMap *map,uint64_t key,int64_t *out_value);
bool u64_lhm_remove(U64LinkedHashMap *map,uint64_t key);
bool u64_lhm_nth(const U64LinkedHashMap *map,size_t index,uint64_t *out_key,int64_t *out_value);
bool u64_lhm_validate(const U64LinkedHashMap *map);
#endif
