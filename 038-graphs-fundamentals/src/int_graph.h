#ifndef INT_GRAPH_H
#define INT_GRAPH_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    size_t from;
    size_t to;
    int weight;
} IntGraphEdge;

typedef struct IntGraph IntGraph;

IntGraph *int_graph_create(size_t vertex_count, bool directed);
void int_graph_free(IntGraph *graph);

size_t int_graph_vertex_count(const IntGraph *graph);
size_t int_graph_edge_count(const IntGraph *graph);
bool int_graph_is_directed(const IntGraph *graph);

bool int_graph_add_edge(IntGraph *graph, size_t from, size_t to, int weight);
bool int_graph_remove_edge(IntGraph *graph, size_t from, size_t to);
bool int_graph_has_edge(
    const IntGraph *graph,
    size_t from,
    size_t to,
    int *out_weight
);

bool int_graph_edge_at(
    const IntGraph *graph,
    size_t index,
    IntGraphEdge *out_edge
);

bool int_graph_degree(const IntGraph *graph, size_t vertex, size_t *out_degree);
bool int_graph_in_degree(const IntGraph *graph, size_t vertex, size_t *out_degree);
bool int_graph_out_degree(const IntGraph *graph, size_t vertex, size_t *out_degree);

bool int_graph_validate(const IntGraph *graph);

#endif
