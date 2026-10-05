#include "int_treap.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntTreap *tree = int_treap_create();
    assert(tree != NULL);

    assert(int_treap_insert_with_priority(tree, 50, 80));
    assert(int_treap_insert_with_priority(tree, 30, 40));
    assert(int_treap_insert_with_priority(tree, 70, 20));

    int root_key = 0;
    uint32_t root_priority = 0;

    assert(int_treap_root(tree, &root_key, &root_priority));

    printf("root=%d priority=%u size=%zu\n",
           root_key,
           root_priority,
           int_treap_size(tree));

    assert(root_key == 70);
    assert(int_treap_validate(tree));

    int_treap_free(tree);
    return 0;
}
