#ifndef INT_R_TREE_H
#define INT_R_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    int64_t x_low;
    int64_t x_high;
    int64_t y_low;
    int64_t y_high;
    uint64_t id;
} IntRRect;

typedef struct IntRTree IntRTree;

IntRTree *int_r_tree_create(void);
void int_r_tree_free(IntRTree *tree);

size_t int_r_tree_size(const IntRTree *tree);
size_t int_r_tree_node_count(const IntRTree *tree);

bool int_r_tree_insert(
    IntRTree *tree,
    IntRRect rectangle
);

bool int_r_tree_query_count(
    const IntRTree *tree,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    size_t *out_count
);

bool int_r_tree_query_report(
    const IntRTree *tree,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    IntRRect *output,
    size_t capacity,
    size_t *out_written
);

bool int_r_tree_validate(const IntRTree *tree);

#endif
