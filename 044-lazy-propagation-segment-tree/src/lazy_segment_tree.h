#ifndef LAZY_SEGMENT_TREE_H
#define LAZY_SEGMENT_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct LazySegmentTree LazySegmentTree;

LazySegmentTree *lazy_segment_tree_create(
    const int64_t *values,
    size_t count
);
void lazy_segment_tree_free(LazySegmentTree *tree);

size_t lazy_segment_tree_size(const LazySegmentTree *tree);

bool lazy_segment_tree_range_add(
    LazySegmentTree *tree,
    size_t left,
    size_t right,
    int64_t delta
);

bool lazy_segment_tree_range_sum(
    const LazySegmentTree *tree,
    size_t left,
    size_t right,
    int64_t *out_sum
);

bool lazy_segment_tree_range_min(
    const LazySegmentTree *tree,
    size_t left,
    size_t right,
    int64_t *out_min
);

bool lazy_segment_tree_point_get(
    const LazySegmentTree *tree,
    size_t index,
    int64_t *out_value
);

bool lazy_segment_tree_validate(const LazySegmentTree *tree);

#endif
