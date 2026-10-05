#ifndef U64_MULTI_H
#define U64_MULTI_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct U64Multiset U64Multiset;
typedef struct U64Multimap U64Multimap;
U64Multiset *u64_multiset_create(void);
void u64_multiset_free(U64Multiset *set);
size_t u64_multiset_distinct(const U64Multiset *set);
size_t u64_multiset_size(const U64Multiset *set);
bool u64_multiset_add(U64Multiset *set,uint64_t key,size_t count);
bool u64_multiset_remove(U64Multiset *set,uint64_t key,size_t count);
size_t u64_multiset_count(const U64Multiset *set,uint64_t key);
bool u64_multiset_nth_distinct(const U64Multiset *set,size_t index,uint64_t *key,size_t *count);
bool u64_multiset_validate(const U64Multiset *set);
U64Multimap *u64_multimap_create(void);
void u64_multimap_free(U64Multimap *map);
size_t u64_multimap_size(const U64Multimap *map);
bool u64_multimap_add(U64Multimap *map,uint64_t key,int64_t value);
size_t u64_multimap_count(const U64Multimap *map,uint64_t key);
bool u64_multimap_get_nth(const U64Multimap *map,uint64_t key,size_t occurrence,int64_t *value);
bool u64_multimap_remove_one(U64Multimap *map,uint64_t key,int64_t value);
size_t u64_multimap_remove_all(U64Multimap *map,uint64_t key);
bool u64_multimap_validate(const U64Multimap *map);
#endif
