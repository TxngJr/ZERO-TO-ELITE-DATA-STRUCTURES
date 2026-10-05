#ifndef GRAPH_REPR_H
#define GRAPH_REPR_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    GRAPH_REPR_EDGE_LIST = 1,
    GRAPH_REPR_ADJ_MATRIX = 2,
    GRAPH_REPR_ADJ_LIST = 3
} GraphReprKind;

typedef struct GraphRepr GraphRepr;

typedef bool (*graph_neighbor_visit_fn)(
    size_t neighbor,
    int weight,
    void *context
);

GraphRepr *graph_repr_create(
    size_t vertex_count,
    bool directed,
    GraphReprKind kind
);
void graph_repr_free(GraphRepr *graph);

size_t graph_repr_vertex_count(const GraphRepr *graph);
size_t graph_repr_edge_count(const GraphRepr *graph);
GraphReprKind graph_repr_kind(const GraphRepr *graph);

bool graph_repr_add_edge(GraphRepr *graph, size_t from, size_t to, int weight);
bool graph_repr_remove_edge(GraphRepr *graph, size_t from, size_t to);
bool graph_repr_has_edge(
    const GraphRepr *graph,
    size_t from,
    size_t to,
    int *out_weight
);

bool graph_repr_for_each_neighbor(
    const GraphRepr *graph,
    size_t vertex,
    graph_neighbor_visit_fn visitor,
    void *context
);

size_t graph_repr_memory_bytes(const GraphRepr *graph);
bool graph_repr_validate(const GraphRepr *graph);

#endif
