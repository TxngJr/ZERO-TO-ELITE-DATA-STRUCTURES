#ifndef CONCURRENT_HASH_MAP_H
#define CONCURRENT_HASH_MAP_H
#include <stdbool.h>
#include <stddef.h>
typedef struct ConcurrentHashMap ConcurrentHashMap;
ConcurrentHashMap *chm_create(size_t initial_buckets);
void chm_free(ConcurrentHashMap *map);
bool chm_put(ConcurrentHashMap *map,int key,int value,bool *out_inserted);
bool chm_get(ConcurrentHashMap *map,int key,int *out_value,bool *out_found);
bool chm_remove(ConcurrentHashMap *map,int key,bool *out_removed);
size_t chm_size(const ConcurrentHashMap *map);
bool chm_bucket_count(ConcurrentHashMap *map,size_t *out_count);
size_t chm_resize_count(const ConcurrentHashMap *map);
bool chm_validate_quiescent(ConcurrentHashMap *map);
#endif
