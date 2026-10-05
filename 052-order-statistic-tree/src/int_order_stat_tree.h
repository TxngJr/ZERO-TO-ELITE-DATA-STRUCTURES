#ifndef INT_ORDER_STAT_TREE_H
#define INT_ORDER_STAT_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct IntOrderStatTree IntOrderStatTree;

IntOrderStatTree *int_order_stat_tree_create(void);
void int_order_stat_tree_free(IntOrderStatTree *tree);

size_t int_order_stat_tree_size(const IntOrderStatTree *tree);

bool int_order_stat_tree_insert(
    IntOrderStatTree *tree,
    int64_t key
);

bool int_order_stat_tree_remove(
    IntOrderStatTree *tree,
    int64_t key
);

bool int_order_stat_tree_contains(
    const IntOrderStatTree *tree,
    int64_t key
);

bool int_order_stat_tree_select(
    const IntOrderStatTree *tree,
    size_t rank,
    int64_t *out_key
);

bool int_order_stat_tree_rank(
    const IntOrderStatTree *tree,
    int64_t key,
    size_t *out_rank
);

bool int_order_stat_tree_count_range(
    const IntOrderStatTree *tree,
    int64_t low,
    int64_t high,
    size_t *out_count
);

bool int_order_stat_tree_validate(
    const IntOrderStatTree *tree
);

#endif
