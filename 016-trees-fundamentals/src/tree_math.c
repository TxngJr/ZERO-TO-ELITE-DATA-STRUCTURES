#include "tree_math.h"

#include <stdint.h>

bool perfect_binary_tree_leaves(size_t edge_height, size_t *out_leaves) {
    if (out_leaves == NULL) return false;
    if (edge_height >= sizeof(size_t) * 8) return false;

    *out_leaves = (size_t)1 << edge_height;
    return true;
}

bool perfect_binary_tree_nodes(size_t edge_height, size_t *out_nodes) {
    if (out_nodes == NULL) return false;

    size_t leaves = 0;
    if (!perfect_binary_tree_leaves(edge_height, &leaves)) return false;
    if (leaves > SIZE_MAX / 2) return false;

    *out_nodes = leaves * 2 - 1;
    return true;
}

bool binary_tree_min_height(size_t node_count, size_t *out_height) {
    if (node_count == 0 || out_height == NULL) return false;

    size_t height = 0;
    size_t max_nodes = 1;

    while (max_nodes < node_count) {
        if (max_nodes > (SIZE_MAX - 1) / 2) {
            ++height;
            break;
        }

        max_nodes = max_nodes * 2 + 1;
        ++height;
    }

    *out_height = height;
    return true;
}

bool binary_tree_max_height(size_t node_count, size_t *out_height) {
    if (node_count == 0 || out_height == NULL) return false;
    *out_height = node_count - 1;
    return true;
}
