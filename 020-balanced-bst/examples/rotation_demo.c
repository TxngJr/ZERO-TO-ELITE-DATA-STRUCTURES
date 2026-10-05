#include "rot_bst.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    RotBST *tree = rot_bst_create();
    assert(tree != NULL);

    const int keys[] = {10, 5, 20, 15, 30};

    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; ++i) {
        assert(rot_bst_insert(tree, keys[i]));
    }

    int before[5];
    int after[5];
    size_t n1 = 0;
    size_t n2 = 0;

    assert(rot_bst_inorder(tree, before, 5, &n1));
    assert(rot_bst_rotate_left(tree, 10));
    assert(rot_bst_validate(tree));
    assert(rot_bst_inorder(tree, after, 5, &n2));

    assert(n1 == n2);
    assert(memcmp(before, after, n1 * sizeof before[0]) == 0);

    printf("root after left rotation=%d\n", rot_bst_node_key(rot_bst_root(tree)));

    assert(rot_bst_rotate_right(tree, 20));
    assert(rot_bst_validate(tree));

    rot_bst_free(tree);
    return 0;
}
