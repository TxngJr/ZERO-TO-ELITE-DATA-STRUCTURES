#include "int_octree.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntOctree *tree=int_octree_create(
        -100,100,-100,100,-100,100,4,16
    );
    assert(tree!=NULL);

    const IntOctPoint points[]={
        {-10,-10,-10,1},
        {10,10,10,2},
        {20,-30,40,3},
        {10,10,10,4}
    };

    for(size_t i=0;i<4;++i) {
        assert(int_octree_insert(tree,points[i]));
    }

    size_t count=0;

    assert(int_octree_query_count(
        tree,0,50,0,50,0,50,&count
    ));

    printf(
        "count=%zu nodes=%zu\n",
        count,int_octree_node_count(tree)
    );

    assert(int_octree_validate(tree));
    int_octree_free(tree);
    return 0;
}
