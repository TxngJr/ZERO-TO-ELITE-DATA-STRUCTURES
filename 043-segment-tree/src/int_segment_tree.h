#ifndef INT_SEGMENT_TREE_H
#define INT_SEGMENT_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct IntSegmentTree IntSegmentTree;

IntSegmentTree *int_segment_tree_create(
    const int64_t *values,
    size_t count
);
void int_segment_tree_free(IntSegmentTree *tree);

size_t int_segment_tree_size(const IntSegmentTree *tree);

bool int_segment_tree_point_set(
    IntSegmentTree *tree,
    size_t index,
    int64_t value
);
bool int_segment_tree_point_get(
    const IntSegmentTree *tree,
    size_t index,
    int64_t *out_value
);

bool int_segment_tree_range_sum(
    const IntSegmentTree *tree,
    size_t left,
    size_t right,
    int64_t *out_sum
);
bool int_segment_tree_range_min(
    const IntSegmentTree *tree,
    size_t left,
    size_t right,
    int64_t *out_min
);

bool int_segment_tree_validate(const IntSegmentTree *tree);

#endif
