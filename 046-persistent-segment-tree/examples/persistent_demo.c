#include "persistent_segment_tree.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

static void print_version(
    const PersistentSegmentTree *tree,
    size_t version
) {
    printf("v%zu:",version);

    for(size_t i=0;i<persistent_segment_tree_size(tree);++i) {
        int64_t value=0;
        assert(persistent_segment_tree_point_get(
            tree,version,i,&value
        ));
        printf(" %" PRId64,value);
    }

    putchar('\n');
}

int main(void) {
    const int64_t values[]={1,2,3,4};

    PersistentSegmentTree *tree=
        persistent_segment_tree_create(values,4);
    assert(tree!=NULL);

    size_t v1=0;
    size_t v2=0;
    size_t v3=0;

    assert(persistent_segment_tree_point_set(
        tree,0,1,20,&v1
    ));
    assert(persistent_segment_tree_point_set(
        tree,0,3,40,&v2
    ));
    assert(persistent_segment_tree_point_set(
        tree,v1,0,10,&v3
    ));

    print_version(tree,0);
    print_version(tree,v1);
    print_version(tree,v2);
    print_version(tree,v3);

    printf("versions=%zu nodes=%zu\n",
           persistent_segment_tree_version_count(tree),
           persistent_segment_tree_node_count(tree));

    assert(persistent_segment_tree_validate(tree));
    persistent_segment_tree_free(tree);
    return 0;
}
