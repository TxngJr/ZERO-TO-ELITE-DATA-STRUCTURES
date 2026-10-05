#ifndef DYNAMIC_SEGMENT_TREE_H
#define DYNAMIC_SEGMENT_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct DynamicSegmentTree DynamicSegmentTree;

DynamicSegmentTree *dynamic_segment_tree_create(
    uint64_t domain_left,
    uint64_t domain_right
);
void dynamic_segment_tree_free(DynamicSegmentTree *tree);

uint64_t dynamic_segment_tree_domain_left(
    const DynamicSegmentTree *tree
);
uint64_t dynamic_segment_tree_domain_right(
    const DynamicSegmentTree *tree
);
size_t dynamic_segment_tree_node_count(
    const DynamicSegmentTree *tree
);

bool dynamic_segment_tree_point_add(
    DynamicSegmentTree *tree,
    uint64_t coordinate,
    int64_t delta
);

bool dynamic_segment_tree_point_get(
    const DynamicSegmentTree *tree,
    uint64_t coordinate,
    int64_t *out_value
);

bool dynamic_segment_tree_range_sum(
    const DynamicSegmentTree *tree,
    uint64_t left,
    uint64_t right,
    int64_t *out_sum
);

bool dynamic_segment_tree_validate(
    const DynamicSegmentTree *tree
);

#endif
