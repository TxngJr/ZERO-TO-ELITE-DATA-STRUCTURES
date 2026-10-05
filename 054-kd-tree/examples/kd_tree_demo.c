#include "int_kd_tree.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    const IntKDPoint points[]={
        {2,3,1},{5,4,2},{9,6,3},
        {4,7,4},{8,1,5},{7,2,6}
    };

    IntKDTree *tree=int_kd_tree_create(
        points,sizeof points/sizeof points[0]
    );
    assert(tree!=NULL);

    size_t count=0;

    assert(int_kd_tree_query_count(
        tree,3,9,1,6,&count
    ));

    IntKDPoint nearest;
    long double distance=0.0L;

    assert(int_kd_tree_nearest(
        tree,6,3,&nearest,&distance
    ));

    printf(
        "rectangle_count=%zu nearest=id=%" PRIu64
        " (%" PRId64 ",%" PRId64 ") dist2=%.0Lf\n",
        count,
        nearest.id,
        nearest.x,
        nearest.y,
        distance
    );

    assert(int_kd_tree_validate(tree));
    int_kd_tree_free(tree);
    return 0;
}
