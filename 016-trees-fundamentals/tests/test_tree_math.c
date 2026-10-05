#include "tree_math.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    size_t value = 0;

    assert(perfect_binary_tree_nodes(0, &value) && value == 1);
    assert(perfect_binary_tree_nodes(1, &value) && value == 3);
    assert(perfect_binary_tree_nodes(2, &value) && value == 7);
    assert(perfect_binary_tree_nodes(3, &value) && value == 15);

    assert(perfect_binary_tree_leaves(0, &value) && value == 1);
    assert(perfect_binary_tree_leaves(4, &value) && value == 16);

    assert(binary_tree_min_height(1, &value) && value == 0);
    assert(binary_tree_min_height(2, &value) && value == 1);
    assert(binary_tree_min_height(3, &value) && value == 1);
    assert(binary_tree_min_height(4, &value) && value == 2);
    assert(binary_tree_min_height(7, &value) && value == 2);
    assert(binary_tree_min_height(8, &value) && value == 3);

    assert(binary_tree_max_height(1, &value) && value == 0);
    assert(binary_tree_max_height(10, &value) && value == 9);

    assert(!binary_tree_min_height(0, &value));
    assert(!binary_tree_max_height(0, &value));
    assert(!perfect_binary_tree_nodes(1, NULL));

    puts("tree math tests passed");
    return 0;
}
