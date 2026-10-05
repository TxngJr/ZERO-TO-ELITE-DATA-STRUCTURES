#ifndef INT_QUADTREE_H
#define INT_QUADTREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    int64_t x;
    int64_t y;
    uint64_t id;
} IntQuadPoint;

typedef struct IntQuadtree IntQuadtree;

IntQuadtree *int_quadtree_create(
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    size_t bucket_capacity,
    size_t max_depth
);

void int_quadtree_free(IntQuadtree *tree);

size_t int_quadtree_size(const IntQuadtree *tree);
size_t int_quadtree_node_count(const IntQuadtree *tree);

bool int_quadtree_insert(
    IntQuadtree *tree,
    IntQuadPoint point
);

bool int_quadtree_query_count(
    const IntQuadtree *tree,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    size_t *out_count
);

bool int_quadtree_query_report(
    const IntQuadtree *tree,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    IntQuadPoint *output,
    size_t capacity,
    size_t *out_written
);

bool int_quadtree_validate(const IntQuadtree *tree);

#endif
