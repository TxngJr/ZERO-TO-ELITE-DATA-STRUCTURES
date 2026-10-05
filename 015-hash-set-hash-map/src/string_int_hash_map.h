#ifndef STRING_INT_HASH_MAP_H
#define STRING_INT_HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct StringIntHashMap StringIntHashMap;

StringIntHashMap *string_int_hash_map_create(void);
void string_int_hash_map_free(StringIntHashMap *map);

size_t string_int_hash_map_size(const StringIntHashMap *map);
size_t string_int_hash_map_capacity(const StringIntHashMap *map);

bool string_int_hash_map_put(StringIntHashMap *map, const char *key, int value);
bool string_int_hash_map_get(const StringIntHashMap *map, const char *key, int *out_value);
bool string_int_hash_map_contains(const StringIntHashMap *map, const char *key);
bool string_int_hash_map_remove(StringIntHashMap *map, const char *key, int *out_value);
bool string_int_hash_map_validate(const StringIntHashMap *map);

#endif
