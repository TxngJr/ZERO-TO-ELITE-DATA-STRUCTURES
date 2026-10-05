#include "int_avl.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntAVL *tree = int_avl_create();
    assert(tree != NULL);

    for (int key = 1; key <= 20; ++key) {
        assert(int_avl_insert(tree, key));
        assert(int_avl_validate(tree));
    }

    size_t height = 0;
    assert(int_avl_height(tree, &height));

    printf("size=%zu height=%zu contains13=%d\n",
           int_avl_size(tree),
           height,
           int_avl_contains(tree, 13));

    assert(int_avl_remove(tree, 10));
    assert(int_avl_validate(tree));

    int_avl_free(tree);
    return 0;
}
