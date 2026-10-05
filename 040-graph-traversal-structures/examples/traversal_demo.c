#include "graph_traversal.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    GraphRepr *graph=graph_repr_create(
        7,false,GRAPH_REPR_ADJ_LIST
    );
    GraphTraversal *traversal=graph_traversal_create(7);

    assert(graph!=NULL && traversal!=NULL);

    assert(graph_repr_add_edge(graph,0,1,1));
    assert(graph_repr_add_edge(graph,0,2,1));
    assert(graph_repr_add_edge(graph,1,3,1));
    assert(graph_repr_add_edge(graph,2,4,1));
    assert(graph_repr_add_edge(graph,3,5,1));
    assert(graph_repr_add_edge(graph,4,5,1));

    assert(graph_traversal_bfs(traversal,graph,0));
    assert(graph_traversal_validate(traversal,graph));

    puts("BFS order:");
    for(size_t i=0;i<graph_traversal_order_count(traversal);++i) {
        size_t v=0;
        size_t depth=0;

        assert(graph_traversal_order_at(traversal,i,&v));
        assert(graph_traversal_depth(traversal,v,&depth));

        printf("  vertex=%zu depth=%zu\n",v,depth);
    }

    size_t path[7];
    size_t written=0;

    assert(graph_traversal_path_to(
        traversal,5,path,7,&written
    ));

    printf("path to 5:");
    for(size_t i=0;i<written;++i)printf(" %zu",path[i]);
    putchar('\n');

    graph_traversal_free(traversal);
    graph_repr_free(graph);
    return 0;
}
