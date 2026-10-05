#ifndef INT_CARTESIAN_TREE_H
#define INT_CARTESIAN_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct IntCartesianTree IntCartesianTree;

IntCartesianTree *int_cartesian_tree_create(
    const int64_t *values,
    size_t count
);

void int_cartesian_tree_free(IntCartesianTree *tree);

size_t int_cartesian_tree_size(const IntCartesianTree *tree);

bool int_cartesian_tree_root(
    const IntCartesianTree *tree,
    size_t *out_index
);

bool int_cartesian_tree_parent(
    const IntCartesianTree *tree,
    size_t index,
    size_t *out_parent
);

bool int_cartesian_tree_left(
    const IntCartesianTree *tree,
    size_t index,
    size_t *out_left
);

bool int_cartesian_tree_right(
    const IntCartesianTree *tree,
    size_t index,
    size_t *out_right
);

bool int_cartesian_tree_height(
    const IntCartesianTree *tree,
    size_t *out_edge_height
);

bool int_cartesian_tree_range_min(
    const IntCartesianTree *tree,
    size_t left,
    size_t right,
    size_t *out_index,
    int64_t *out_value
);

bool int_cartesian_tree_validate(
    const IntCartesianTree *tree
);

#endif
