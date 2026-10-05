#ifndef INT_SPATIAL_HASH_H
#define INT_SPATIAL_HASH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    int64_t x;
    int64_t y;
    uint64_t id;
} IntSpatialPoint;

typedef struct IntSpatialHash IntSpatialHash;

IntSpatialHash *int_spatial_hash_create(int64_t cell_size);
void int_spatial_hash_free(IntSpatialHash *hash);

size_t int_spatial_hash_size(const IntSpatialHash *hash);
size_t int_spatial_hash_cell_count(const IntSpatialHash *hash);

bool int_spatial_hash_insert(
    IntSpatialHash *hash,
    IntSpatialPoint point
);

bool int_spatial_hash_query_count(
    const IntSpatialHash *hash,
    int64_t x_low,int64_t x_high,
    int64_t y_low,int64_t y_high,
    size_t *out_count
);

bool int_spatial_hash_query_report(
    const IntSpatialHash *hash,
    int64_t x_low,int64_t x_high,
    int64_t y_low,int64_t y_high,
    IntSpatialPoint *output,
    size_t capacity,
    size_t *out_written
);

bool int_spatial_hash_validate(const IntSpatialHash *hash);

#endif
