#ifndef DISK_SORTED_INDEX_H
#define DISK_SORTED_INDEX_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct DiskSortedIndex DiskSortedIndex;
typedef struct { uint64_t key; int64_t value; } DiskRecord;

bool dsi_create(const char *path,const DiskRecord *records,size_t count);
DiskSortedIndex *dsi_open(const char *path);
void dsi_close(DiskSortedIndex *index);
size_t dsi_count(const DiskSortedIndex *index);
bool dsi_get(DiskSortedIndex *index,uint64_t key,int64_t *out_value,bool *out_found);
bool dsi_range_collect(DiskSortedIndex *index,uint64_t low,uint64_t high,
                       DiskRecord *out,size_t out_capacity,size_t *out_count);
void dsi_reset_io_counters(DiskSortedIndex *index);
size_t dsi_logical_seeks(const DiskSortedIndex *index);
size_t dsi_record_reads(const DiskSortedIndex *index);
bool dsi_validate(DiskSortedIndex *index);

#endif
