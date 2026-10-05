#include "persistent_segment_tree.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t n=65536;
    const size_t updates=50000;

    int64_t *initial=calloc(n,sizeof *initial);
    if(initial==NULL)return 1;

    PersistentSegmentTree *tree=
        persistent_segment_tree_create(initial,n);
    if(tree==NULL)return 1;

    uint32_t rng=123456789u;
    size_t latest=0;

    const size_t initial_nodes=
        persistent_segment_tree_node_count(tree);

    struct timespec a,b;
    timespec_get(&a,TIME_UTC);

    for(size_t i=0;i<updates;++i) {
        rng=rng*1664525u+1013904223u;
        const size_t index=rng%n;

        size_t next=0;

        if(!persistent_segment_tree_point_set(
                tree,
                latest,
                index,
                (int64_t)i,
                &next
            )) {
            return 1;
        }

        latest=next;
    }

    timespec_get(&b,TIME_UTC);

    const size_t final_nodes=
        persistent_segment_tree_node_count(tree);

    printf(
        "n=%zu updates=%zu initial_nodes=%zu final_nodes=%zu nodes_per_update=%.3f seconds=%.9f\n",
        n,
        updates,
        initial_nodes,
        final_nodes,
        (double)(final_nodes-initial_nodes)/(double)updates,
        elapsed(a,b)
    );

    puts("Full snapshot copying would store n values per version; path copying adds only one root-to-leaf path.");

    persistent_segment_tree_free(tree);
    free(initial);
    return 0;
}
