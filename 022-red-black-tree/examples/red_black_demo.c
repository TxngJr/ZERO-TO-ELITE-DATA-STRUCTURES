#include "int_red_black_tree.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntRedBlackTree *tree = int_rbt_create();
    assert(tree != NULL);

    const int keys[] = {10,20,30,15,25,5,1,7};

    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; ++i) {
        assert(int_rbt_insert(tree, keys[i]));
        assert(int_rbt_validate(tree));
    }

    size_t height = 0;
    assert(int_rbt_height(tree, &height));

    printf("size=%zu height=%zu contains25=%d\n",
           int_rbt_size(tree),
           height,
           int_rbt_contains(tree, 25));

    assert(int_rbt_remove(tree, 20));
    assert(int_rbt_validate(tree));

    int_rbt_free(tree);
    return 0;
}
