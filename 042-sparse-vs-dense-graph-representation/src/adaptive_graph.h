#ifndef ADAPTIVE_GRAPH_H
#define ADAPTIVE_GRAPH_H

#include "graph_repr.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct AdaptiveGraph AdaptiveGraph;

AdaptiveGraph *adaptive_graph_create(
    size_t vertex_count,
    bool directed,
    unsigned promote_percent,
    unsigned demote_percent
);
void adaptive_graph_free(AdaptiveGraph *graph);

size_t adaptive_graph_vertex_count(const AdaptiveGraph *graph);
size_t adaptive_graph_edge_count(const AdaptiveGraph *graph);
GraphReprKind adaptive_graph_backend_kind(const AdaptiveGraph *graph);
size_t adaptive_graph_switch_count(const AdaptiveGraph *graph);
double adaptive_graph_density_percent(const AdaptiveGraph *graph);
size_t adaptive_graph_memory_bytes(const AdaptiveGraph *graph);

bool adaptive_graph_add_edge(
    AdaptiveGraph *graph,
    size_t from,
    size_t to,
    int weight
);
bool adaptive_graph_remove_edge(
    AdaptiveGraph *graph,
    size_t from,
    size_t to
);
bool adaptive_graph_has_edge(
    const AdaptiveGraph *graph,
    size_t from,
    size_t to,
    int *out_weight
);
bool adaptive_graph_for_each_neighbor(
    const AdaptiveGraph *graph,
    size_t vertex,
    graph_neighbor_visit_fn visitor,
    void *context
);

bool adaptive_graph_rebalance(AdaptiveGraph *graph);
bool adaptive_graph_validate(const AdaptiveGraph *graph);

#endif
