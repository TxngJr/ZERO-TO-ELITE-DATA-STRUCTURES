#include "int_graph.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntGraph *graph = int_graph_create(6, false);
    assert(graph != NULL);

    assert(int_graph_add_edge(graph,0,1,5));
    assert(int_graph_add_edge(graph,0,2,7));
    assert(int_graph_add_edge(graph,1,3,2));
    assert(int_graph_add_edge(graph,2,3,4));
    assert(int_graph_add_edge(graph,3,4,1));

    size_t sum = 0;

    for (size_t v = 0; v < 6; ++v) {
        size_t degree = 0;
        assert(int_graph_degree(graph,v,&degree));
        sum += degree;
        printf("degree(%zu)=%zu\n",v,degree);
    }

    printf("V=%zu E=%zu degree_sum=%zu\n",
           int_graph_vertex_count(graph),
           int_graph_edge_count(graph),
           sum);

    assert(sum == 2 * int_graph_edge_count(graph));
    assert(int_graph_validate(graph));

    int_graph_free(graph);
    return 0;
}
