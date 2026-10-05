#include "int_btree.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntBTree *tree=int_btree_create(3);
    assert(tree!=NULL);

    for(int i=1;i<=30;++i) {
        assert(int_btree_insert(tree,i));
        assert(int_btree_validate(tree));
    }

    size_t height=0;
    assert(int_btree_height(tree,&height));

    printf("size=%zu t=%zu height=%zu\n",
           int_btree_size(tree),
           int_btree_minimum_degree(tree),
           height);

    for(int i=2;i<=30;i+=2) {
        assert(int_btree_remove(tree,i));
        assert(int_btree_validate(tree));
    }

    int_btree_free(tree);
    return 0;
}
