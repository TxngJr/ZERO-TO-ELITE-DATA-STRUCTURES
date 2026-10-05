#ifndef INT_INTERVAL_TREE_H
#define INT_INTERVAL_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct IntIntervalTree IntIntervalTree;

typedef struct {
    int64_t low;
    int64_t high;
} IntInterval;

IntIntervalTree *int_interval_tree_create(void);
void int_interval_tree_free(IntIntervalTree *tree);

size_t int_interval_tree_size(const IntIntervalTree *tree);

bool int_interval_tree_insert(
    IntIntervalTree *tree,
    int64_t low,
    int64_t high
);

bool int_interval_tree_remove(
    IntIntervalTree *tree,
    int64_t low,
    int64_t high
);

bool int_interval_tree_contains(
    const IntIntervalTree *tree,
    int64_t low,
    int64_t high
);

bool int_interval_tree_find_overlap(
    const IntIntervalTree *tree,
    int64_t query_low,
    int64_t query_high,
    IntInterval *out_interval
);

bool int_interval_tree_count_overlaps(
    const IntIntervalTree *tree,
    int64_t query_low,
    int64_t query_high,
    size_t *out_count
);

bool int_interval_tree_validate(
    const IntIntervalTree *tree
);

#endif
