#include "int_bplus_tree.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntBPlusTree *tree=int_bplus_tree_create(3);
    assert(tree!=NULL);

    for(int i=1;i<=50;++i) {
        assert(int_bplus_tree_insert(tree,i));
        assert(int_bplus_tree_validate(tree));
    }

    int output[32];
    size_t written=0;

    assert(int_bplus_tree_range(
        tree,17,29,output,32,&written
    ));

    printf("range 17..29:");
    for(size_t i=0;i<written;++i)printf(" %d",output[i]);
    putchar('\n');

    int_bplus_tree_free(tree);
    return 0;
}
