#ifndef GRAPH_TRAVERSAL_H
#define GRAPH_TRAVERSAL_H

#include "graph_repr.h"

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    GRAPH_TRAVERSAL_NONE = 0,
    GRAPH_TRAVERSAL_BFS = 1,
    GRAPH_TRAVERSAL_DFS = 2
} GraphTraversalMode;

typedef struct GraphTraversal GraphTraversal;

GraphTraversal *graph_traversal_create(size_t vertex_count);
void graph_traversal_free(GraphTraversal *traversal);

bool graph_traversal_bfs(
    GraphTraversal *traversal,
    const GraphRepr *graph,
    size_t source
);

bool graph_traversal_dfs(
    GraphTraversal *traversal,
    const GraphRepr *graph,
    size_t source
);

size_t graph_traversal_order_count(const GraphTraversal *traversal);
bool graph_traversal_order_at(
    const GraphTraversal *traversal,
    size_t index,
    size_t *out_vertex
);
bool graph_traversal_visited(
    const GraphTraversal *traversal,
    size_t vertex,
    bool *out_visited
);
bool graph_traversal_parent(
    const GraphTraversal *traversal,
    size_t vertex,
    size_t *out_parent
);
bool graph_traversal_depth(
    const GraphTraversal *traversal,
    size_t vertex,
    size_t *out_depth
);

bool graph_traversal_path_to(
    const GraphTraversal *traversal,
    size_t target,
    size_t *output,
    size_t capacity,
    size_t *out_written
);

GraphTraversalMode graph_traversal_mode(
    const GraphTraversal *traversal
);

bool graph_traversal_validate(
    const GraphTraversal *traversal,
    const GraphRepr *graph
);

#endif
