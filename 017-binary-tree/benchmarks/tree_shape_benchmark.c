#include "int_binary_tree.h"

#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

static IntBinaryTree *build_chain(size_t n) {
    IntBinaryTree *tree = int_binary_tree_create();
    if (tree == NULL) return NULL;

    IntBinaryNode *node = NULL;
    if (!int_binary_tree_set_root(tree, 0, &node)) return NULL;

    for (size_t i = 1; i < n; ++i) {
        IntBinaryNode *next = NULL;
        if (!int_binary_tree_add_right(tree, node, (int)i, &next)) return NULL;
        node = next;
    }

    return tree;
}

int main(void) {
    const size_t n = 2000;
    IntBinaryTree *chain = build_chain(n);
    if (chain == NULL) return 1;

    struct timespec a, b;
    size_t height = 0;

    timespec_get(&a, TIME_UTC);
    if (!int_binary_tree_height(chain, &height)) return 1;
    timespec_get(&b, TIME_UTC);

    printf("chain nodes=%zu height=%zu height_scan_seconds=%.9f\n",
           n,
           height,
           elapsed(a, b));

    puts("The benchmark emphasizes shape/depth; construction includes defensive membership checks.");

    int_binary_tree_free(chain);
    return 0;
}
