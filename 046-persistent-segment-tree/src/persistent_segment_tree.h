#ifndef PERSISTENT_SEGMENT_TREE_H
#define PERSISTENT_SEGMENT_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct PersistentSegmentTree PersistentSegmentTree;

PersistentSegmentTree *persistent_segment_tree_create(
    const int64_t *values,
    size_t count
);
void persistent_segment_tree_free(
    PersistentSegmentTree *tree
);

size_t persistent_segment_tree_size(
    const PersistentSegmentTree *tree
);
size_t persistent_segment_tree_version_count(
    const PersistentSegmentTree *tree
);
size_t persistent_segment_tree_node_count(
    const PersistentSegmentTree *tree
);

bool persistent_segment_tree_point_set(
    PersistentSegmentTree *tree,
    size_t base_version,
    size_t index,
    int64_t value,
    size_t *out_new_version
);

bool persistent_segment_tree_point_get(
    const PersistentSegmentTree *tree,
    size_t version,
    size_t index,
    int64_t *out_value
);

bool persistent_segment_tree_range_sum(
    const PersistentSegmentTree *tree,
    size_t version,
    size_t left,
    size_t right,
    int64_t *out_sum
);

bool persistent_segment_tree_range_min(
    const PersistentSegmentTree *tree,
    size_t version,
    size_t left,
    size_t right,
    int64_t *out_min
);

bool persistent_segment_tree_validate(
    const PersistentSegmentTree *tree
);

#endif
