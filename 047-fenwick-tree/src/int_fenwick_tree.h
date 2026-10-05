#ifndef INT_FENWICK_TREE_H
#define INT_FENWICK_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct IntFenwickTree IntFenwickTree;

IntFenwickTree *int_fenwick_tree_create(
    const int64_t *values,
    size_t count
);
void int_fenwick_tree_free(IntFenwickTree *tree);

size_t int_fenwick_tree_size(const IntFenwickTree *tree);

bool int_fenwick_tree_point_add(
    IntFenwickTree *tree,
    size_t index,
    int64_t delta
);
bool int_fenwick_tree_point_set(
    IntFenwickTree *tree,
    size_t index,
    int64_t value
);
bool int_fenwick_tree_point_get(
    const IntFenwickTree *tree,
    size_t index,
    int64_t *out_value
);

bool int_fenwick_tree_prefix_sum(
    const IntFenwickTree *tree,
    size_t end,
    int64_t *out_sum
);
bool int_fenwick_tree_range_sum(
    const IntFenwickTree *tree,
    size_t left,
    size_t right,
    int64_t *out_sum
);

bool int_fenwick_tree_validate(
    const IntFenwickTree *tree
);

#endif
