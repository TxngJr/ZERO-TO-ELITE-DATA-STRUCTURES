#include "int_bst.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    const int keys[] = {8,3,10,1,6,14,4,7,13};

    IntBST *tree = int_bst_create();
    assert(tree != NULL);

    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; ++i) {
        assert(int_bst_insert(tree, keys[i], NULL));
    }

    int values[9];
    size_t written = 0;
    assert(int_bst_inorder(tree, values, 9, &written));

    printf("inorder:");
    for (size_t i = 0; i < written; ++i) printf(" %d", values[i]);
    putchar('\n');

    IntBSTNode *n6 = int_bst_find(tree, 6);
    assert(n6 != NULL);

    IntBSTNode *succ = int_bst_successor(n6);
    IntBSTNode *pred = int_bst_predecessor(n6);

    printf("key=6 predecessor=%d successor=%d\n",
           int_bst_node_key(pred),
           int_bst_node_key(succ));

    assert(int_bst_validate(tree));
    int_bst_free(tree);
    return 0;
}
