#ifndef CACHE_SIM_H
#define CACHE_SIM_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct CacheSim CacheSim;
typedef struct {
    uint64_t accesses;
    uint64_t hits;
    uint64_t misses;
    uint64_t evictions;
} CacheStats;
CacheSim *cache_sim_create(size_t line_size,size_t set_count,size_t associativity);
void cache_sim_free(CacheSim *sim);
bool cache_sim_access(CacheSim *sim,uint64_t address,bool *out_hit);
bool cache_sim_access_range(CacheSim *sim,const uint64_t *addresses,size_t count);
void cache_sim_reset(CacheSim *sim);
CacheStats cache_sim_stats(const CacheSim *sim);
size_t cache_sim_line_size(const CacheSim *sim);
size_t cache_sim_set_count(const CacheSim *sim);
size_t cache_sim_associativity(const CacheSim *sim);
bool cache_sim_validate(const CacheSim *sim);
#endif
