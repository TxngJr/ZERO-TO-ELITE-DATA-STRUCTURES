#include "tree_traversal.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

int main(void) {
    const size_t n = 16383;

    IntBinaryTree *tree = int_binary_tree_create();
    IntBinaryNode **nodes = malloc(n * sizeof *nodes);
    int *output = malloc(n * sizeof *output);

    if (tree == NULL || nodes == NULL || output == NULL) return 1;

    if (!int_binary_tree_set_root(tree, 0, &nodes[0])) return 1;

    for (size_t i = 1; i < n; ++i) {
        const size_t parent = (i - 1) / 2;
        bool ok = (i & 1U)
            ? int_binary_tree_add_left(tree, nodes[parent], (int)i, &nodes[i])
            : int_binary_tree_add_right(tree, nodes[parent], (int)i, &nodes[i]);

        if (!ok) return 1;
    }

    size_t written = 0;
    struct timespec a, b;

    timespec_get(&a, TIME_UTC);
    if (!tree_preorder_recursive(tree, output, n, &written)) return 1;
    timespec_get(&b, TIME_UTC);
    const double recursive_time = elapsed(a, b);

    timespec_get(&a, TIME_UTC);
    if (!tree_preorder_iterative(tree, output, n, &written)) return 1;
    timespec_get(&b, TIME_UTC);
    const double iterative_time = elapsed(a, b);

    printf("nodes=%zu recursive=%.9f iterative=%.9f\n",
           n,
           recursive_time,
           iterative_time);

    puts("Both are Theta(n); absolute timing depends on compiler, allocation, CPU and cache.");

    free(output);
    free(nodes);
    int_binary_tree_free(tree);
    return 0;
}
