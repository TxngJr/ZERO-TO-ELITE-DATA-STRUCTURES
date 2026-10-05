#include "int_r_tree.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntRTree *tree=int_r_tree_create();
    assert(tree!=NULL);

    const IntRRect rects[]={
        {0,10,0,10,1},
        {20,30,20,30,2},
        {5,15,5,15,3},
        {40,50,0,10,4},
        {22,28,10,25,5}
    };

    for(size_t i=0;i<5;++i) {
        assert(int_r_tree_insert(tree,rects[i]));
    }

    size_t count=0;

    assert(int_r_tree_query_count(
        tree,8,24,8,24,&count
    ));

    printf(
        "overlaps=%zu nodes=%zu\n",
        count,int_r_tree_node_count(tree)
    );

    assert(int_r_tree_validate(tree));
    int_r_tree_free(tree);
    return 0;
}
