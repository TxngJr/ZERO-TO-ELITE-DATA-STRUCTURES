#ifndef FALSE_SHARING_H
#define FALSE_SHARING_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct FsCounterArray FsCounterArray;
FsCounterArray *fs_counter_array_create(size_t count,size_t stride,size_t line_size);
void fs_counter_array_free(FsCounterArray *array);
size_t fs_counter_count(const FsCounterArray *array);
size_t fs_counter_stride(const FsCounterArray *array);
size_t fs_line_size(const FsCounterArray *array);
size_t fs_storage_bytes(const FsCounterArray *array);
bool fs_counters_share_line(const FsCounterArray *array,size_t a,size_t b,bool *out_share);
bool fs_line_occupancy(const FsCounterArray *array,size_t counter_index,size_t *out_count);
bool fs_counter_load(const FsCounterArray *array,size_t index,uint64_t *out_value);
bool fs_counter_add(FsCounterArray *array,size_t index,uint64_t delta);
void fs_counter_reset(FsCounterArray *array);
bool fs_parallel_increment(FsCounterArray *array,size_t thread_count,uint64_t iterations);
bool fs_counter_array_validate(const FsCounterArray *array);
#endif
