#include "int_splay_tree.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntSplayTree *tree = int_splay_create();
    assert(tree != NULL);

    const int keys[] = {10,20,30,5,15,25};

    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; ++i) {
        assert(int_splay_insert(tree, keys[i]));
    }

    assert(int_splay_contains(tree, 10));

    int root = 0;
    assert(int_splay_root_key(tree, &root));

    printf("after access 10: root=%d size=%zu\n",
           root,
           int_splay_size(tree));

    assert(int_splay_remove(tree, 10));
    assert(int_splay_validate(tree));

    int_splay_free(tree);
    return 0;
}
