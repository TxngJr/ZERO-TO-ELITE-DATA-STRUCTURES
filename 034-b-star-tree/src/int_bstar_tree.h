#ifndef INT_BSTAR_TREE_H
#define INT_BSTAR_TREE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntBStarTree IntBStarTree;

IntBStarTree *int_bstar_tree_create(size_t order);
void int_bstar_tree_free(IntBStarTree *tree);

size_t int_bstar_tree_size(const IntBStarTree *tree);
size_t int_bstar_tree_order(const IntBStarTree *tree);
size_t int_bstar_tree_node_count(const IntBStarTree *tree);
double int_bstar_tree_utilization(const IntBStarTree *tree);

bool int_bstar_tree_contains(const IntBStarTree *tree, int key);
bool int_bstar_tree_insert(IntBStarTree *tree, int key);
bool int_bstar_tree_remove(IntBStarTree *tree, int key);

bool int_bstar_tree_height(const IntBStarTree *tree, size_t *out_edge_height);
bool int_bstar_tree_inorder(
    const IntBStarTree *tree,
    int *output,
    size_t capacity,
    size_t *out_written
);
bool int_bstar_tree_validate(const IntBStarTree *tree);

#endif
