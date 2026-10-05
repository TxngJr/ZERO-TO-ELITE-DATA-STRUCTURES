#ifndef INT_OCTREE_H
#define INT_OCTREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    int64_t x;
    int64_t y;
    int64_t z;
    uint64_t id;
} IntOctPoint;

typedef struct IntOctree IntOctree;

IntOctree *int_octree_create(
    int64_t x_low,int64_t x_high,
    int64_t y_low,int64_t y_high,
    int64_t z_low,int64_t z_high,
    size_t bucket_capacity,
    size_t max_depth
);

void int_octree_free(IntOctree *tree);

size_t int_octree_size(const IntOctree *tree);
size_t int_octree_node_count(const IntOctree *tree);

bool int_octree_insert(
    IntOctree *tree,
    IntOctPoint point
);

bool int_octree_query_count(
    const IntOctree *tree,
    int64_t x_low,int64_t x_high,
    int64_t y_low,int64_t y_high,
    int64_t z_low,int64_t z_high,
    size_t *out_count
);

bool int_octree_query_report(
    const IntOctree *tree,
    int64_t x_low,int64_t x_high,
    int64_t y_low,int64_t y_high,
    int64_t z_low,int64_t z_high,
    IntOctPoint *output,
    size_t capacity,
    size_t *out_written
);

bool int_octree_validate(const IntOctree *tree);

#endif
