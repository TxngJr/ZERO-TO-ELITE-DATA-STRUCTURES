#ifndef EXTERNAL_MEMORY_SET_H
#define EXTERNAL_MEMORY_SET_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct ExternalMemorySet ExternalMemorySet;
ExternalMemorySet *ems_build(const uint64_t *sorted_unique,size_t count,size_t block_capacity);
void ems_free(ExternalMemorySet *set);
size_t ems_count(const ExternalMemorySet *set);
size_t ems_block_capacity(const ExternalMemorySet *set);
size_t ems_block_count(const ExternalMemorySet *set);
bool ems_contains(ExternalMemorySet *set,uint64_t key,bool *out_found);
bool ems_range_collect(ExternalMemorySet *set,uint64_t low,uint64_t high,
                       uint64_t *out,size_t out_capacity,size_t *out_count);
void ems_reset_io_counters(ExternalMemorySet *set);
size_t ems_logical_reads(const ExternalMemorySet *set);
size_t ems_logical_writes(const ExternalMemorySet *set);
bool ems_validate(const ExternalMemorySet *set);
#endif
