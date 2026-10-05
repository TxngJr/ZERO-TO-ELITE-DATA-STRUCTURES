#ifndef INT_RANGE_TREE_H
#define INT_RANGE_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    int64_t x;
    int64_t y;
    uint64_t id;
} IntPoint2D;

typedef struct IntRangeTree IntRangeTree;

IntRangeTree *int_range_tree_create(
    const IntPoint2D *points,
    size_t count
);

void int_range_tree_free(IntRangeTree *tree);

size_t int_range_tree_size(const IntRangeTree *tree);

bool int_range_tree_query_count(
    const IntRangeTree *tree,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    size_t *out_count
);

bool int_range_tree_query_report(
    const IntRangeTree *tree,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    IntPoint2D *output,
    size_t capacity,
    size_t *out_written
);

bool int_range_tree_validate(
    const IntRangeTree *tree
);

#endif
