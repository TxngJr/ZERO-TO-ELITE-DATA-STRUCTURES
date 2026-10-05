#ifndef TREE_MATH_H
#define TREE_MATH_H

#include <stdbool.h>
#include <stddef.h>

bool perfect_binary_tree_nodes(size_t edge_height, size_t *out_nodes);
bool perfect_binary_tree_leaves(size_t edge_height, size_t *out_leaves);
bool binary_tree_min_height(size_t node_count, size_t *out_height);
bool binary_tree_max_height(size_t node_count, size_t *out_height);

#endif
