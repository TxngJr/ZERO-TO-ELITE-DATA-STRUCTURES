#ifndef U64_ORDERED_H
#define U64_ORDERED_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct U64OrderedMap U64OrderedMap;
typedef struct U64OrderedSet U64OrderedSet;
U64OrderedMap *u64_ordered_map_create(void);
void u64_ordered_map_free(U64OrderedMap *map);
size_t u64_ordered_map_size(const U64OrderedMap *map);
bool u64_ordered_map_put(U64OrderedMap *map,uint64_t key,int64_t value);
bool u64_ordered_map_get(const U64OrderedMap *map,uint64_t key,int64_t *out);
bool u64_ordered_map_remove(U64OrderedMap *map,uint64_t key);
bool u64_ordered_map_nth(const U64OrderedMap *map,size_t index,uint64_t *key,int64_t *value);
size_t u64_ordered_map_lower_bound(const U64OrderedMap *map,uint64_t key);
bool u64_ordered_map_validate(const U64OrderedMap *map);
U64OrderedSet *u64_ordered_set_create(void);
void u64_ordered_set_free(U64OrderedSet *set);
size_t u64_ordered_set_size(const U64OrderedSet *set);
bool u64_ordered_set_add(U64OrderedSet *set,uint64_t key);
bool u64_ordered_set_contains(const U64OrderedSet *set,uint64_t key);
bool u64_ordered_set_remove(U64OrderedSet *set,uint64_t key);
bool u64_ordered_set_nth(const U64OrderedSet *set,size_t index,uint64_t *key);
bool u64_ordered_set_validate(const U64OrderedSet *set);
#endif
