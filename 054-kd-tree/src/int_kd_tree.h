#ifndef INT_KD_TREE_H
#define INT_KD_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    int64_t x;
    int64_t y;
    uint64_t id;
} IntKDPoint;

typedef struct IntKDTree IntKDTree;

IntKDTree *int_kd_tree_create(
    const IntKDPoint *points,
    size_t count
);

void int_kd_tree_free(IntKDTree *tree);

size_t int_kd_tree_size(const IntKDTree *tree);

bool int_kd_tree_query_count(
    const IntKDTree *tree,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    size_t *out_count
);

bool int_kd_tree_query_report(
    const IntKDTree *tree,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    IntKDPoint *output,
    size_t capacity,
    size_t *out_written
);

bool int_kd_tree_nearest(
    const IntKDTree *tree,
    int64_t query_x,
    int64_t query_y,
    IntKDPoint *out_point,
    long double *out_squared_distance
);

bool int_kd_tree_validate(const IntKDTree *tree);

#endif
