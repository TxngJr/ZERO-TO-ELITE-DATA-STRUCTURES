#include "graph_repr.h"

#include <assert.h>
#include <stdio.h>

static bool print_neighbor(
    size_t neighbor,
    int weight,
    void *context
) {
    (void)context;
    printf(" (%zu,w=%d)",neighbor,weight);
    return true;
}

int main(void) {
    const GraphReprKind kinds[]={
        GRAPH_REPR_EDGE_LIST,
        GRAPH_REPR_ADJ_MATRIX,
        GRAPH_REPR_ADJ_LIST
    };

    for(size_t k=0;k<3;++k) {
        GraphRepr *g=graph_repr_create(5,false,kinds[k]);
        assert(g!=NULL);

        assert(graph_repr_add_edge(g,0,1,10));
        assert(graph_repr_add_edge(g,0,2,20));
        assert(graph_repr_add_edge(g,2,3,30));
        assert(graph_repr_add_edge(g,3,4,40));

        printf("kind=%d bytes=%zu neighbors(0):",
               (int)kinds[k],
               graph_repr_memory_bytes(g));

        assert(graph_repr_for_each_neighbor(
            g,0,print_neighbor,NULL
        ));
        putchar('\n');

        assert(graph_repr_validate(g));
        graph_repr_free(g);
    }

    return 0;
}
