#include "int_range_tree.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    const IntPoint2D points[]={
        {1,5,1},{2,9,2},{4,3,3},{5,12,4},
        {7,7,5},{8,15,6},{9,4,7}
    };

    IntRangeTree *tree=int_range_tree_create(
        points,
        sizeof points/sizeof points[0]
    );
    assert(tree!=NULL);

    size_t count=0;

    assert(int_range_tree_query_count(
        tree,2,9,4,13,&count
    ));

    printf("rectangle count=%zu\n",count);

    IntPoint2D output[7];
    size_t written=0;

    assert(int_range_tree_query_report(
        tree,2,9,4,13,
        output,7,&written
    ));

    for(size_t i=0;i<written;++i) {
        printf(
            "id=%" PRIu64 " (%" PRId64 ",%" PRId64 ")\n",
            output[i].id,
            output[i].x,
            output[i].y
        );
    }

    assert(int_range_tree_validate(tree));
    int_range_tree_free(tree);
    return 0;
}
