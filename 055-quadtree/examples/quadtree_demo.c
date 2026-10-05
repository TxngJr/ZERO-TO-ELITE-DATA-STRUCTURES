#include "int_quadtree.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    IntQuadtree *tree=int_quadtree_create(
        -100,100,-100,100,4,16
    );
    assert(tree!=NULL);

    const IntQuadPoint points[]={
        {-20,-10,1},{10,10,2},{40,40,3},
        {-50,60,4},{10,10,5}
    };

    for(size_t i=0;i<5;++i) {
        assert(int_quadtree_insert(tree,points[i]));
    }

    size_t count=0;

    assert(int_quadtree_query_count(
        tree,0,50,0,50,&count
    ));

    printf(
        "count=%zu nodes=%zu\n",
        count,
        int_quadtree_node_count(tree)
    );

    assert(int_quadtree_validate(tree));
    int_quadtree_free(tree);
    return 0;
}
