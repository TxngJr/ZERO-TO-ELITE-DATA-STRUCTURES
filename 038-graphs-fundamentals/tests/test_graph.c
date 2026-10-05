#include "int_graph.h"

#include <assert.h>
#include <stdio.h>

static void test_undirected(void) {
    IntGraph *g = int_graph_create(7,false);
    assert(g != NULL);

    assert(int_graph_add_edge(g,0,1,10));
    assert(int_graph_add_edge(g,0,2,20));
    assert(int_graph_add_edge(g,2,3,30));
    assert(int_graph_add_edge(g,3,4,40));
    assert(int_graph_add_edge(g,4,5,50));

    assert(!int_graph_add_edge(g,1,0,99));
    assert(!int_graph_add_edge(g,6,6,1));

    int weight = 0;
    assert(int_graph_has_edge(g,1,0,&weight));
    assert(weight == 10);

    size_t degree_sum = 0;

    for (size_t v = 0; v < 7; ++v) {
        size_t d = 0;
        assert(int_graph_degree(g,v,&d));
        degree_sum += d;
    }

    assert(degree_sum == 2 * int_graph_edge_count(g));
    assert(int_graph_validate(g));

    assert(int_graph_remove_edge(g,3,2));
    assert(!int_graph_has_edge(g,2,3,NULL));
    assert(int_graph_validate(g));

    int_graph_free(g);
}

static void test_directed(void) {
    IntGraph *g = int_graph_create(5,true);
    assert(g != NULL);

    assert(int_graph_add_edge(g,0,1,1));
    assert(int_graph_add_edge(g,1,0,2));
    assert(int_graph_add_edge(g,1,2,3));
    assert(int_graph_add_edge(g,2,4,4));
    assert(int_graph_add_edge(g,4,1,5));

    assert(!int_graph_add_edge(g,0,1,9));

    size_t in_sum = 0;
    size_t out_sum = 0;

    for (size_t v = 0; v < 5; ++v) {
        size_t in = 0;
        size_t out = 0;

        assert(int_graph_in_degree(g,v,&in));
        assert(int_graph_out_degree(g,v,&out));

        in_sum += in;
        out_sum += out;
    }

    assert(in_sum == int_graph_edge_count(g));
    assert(out_sum == int_graph_edge_count(g));
    assert(int_graph_validate(g));

    int_graph_free(g);
}

int main(void) {
    test_undirected();
    test_directed();

    puts("Graph fundamentals tests passed");
    return 0;
}
