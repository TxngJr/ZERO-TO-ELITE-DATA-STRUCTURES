#include "rot_bst.h"

#include <stdio.h>

int main(void) {
    const int n = 63;

    RotBST *tree = rot_bst_create();
    if (tree == NULL) return 1;

    for (int i = 1; i <= n; ++i) {
        if (!rot_bst_insert(tree, i)) return 1;
    }

    size_t before = 0;
    size_t after = 0;

    if (!rot_bst_height(tree, &before)) return 1;

    for (int i = 1; i <= n / 2; ++i) {
        if (!rot_bst_rotate_left(tree, i)) return 1;
    }

    if (!rot_bst_height(tree, &after)) return 1;

    printf(
        "nodes=%d height_before=%zu height_after=%zu root=%d\n",
        n,
        before,
        after,
        rot_bst_node_key(rot_bst_root(tree))
    );

    puts("Manual rotations improve this shape but do not constitute an automatic balance policy.");

    rot_bst_free(tree);
    return 0;
}
