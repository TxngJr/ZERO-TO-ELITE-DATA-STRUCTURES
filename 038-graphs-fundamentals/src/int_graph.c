#include "int_graph.h"

#include <stdint.h>
#include <stdlib.h>

struct IntGraph {
    size_t vertex_count;
    bool directed;
    IntGraphEdge *edges;
    size_t edge_count;
    size_t capacity;
};

static void canonicalize(
    const IntGraph *graph,
    size_t *from,
    size_t *to
) {
    if (!graph->directed && *from > *to) {
        const size_t tmp = *from;
        *from = *to;
        *to = tmp;
    }
}

IntGraph *int_graph_create(
    size_t vertex_count,
    bool directed
) {
    IntGraph *graph = calloc(1, sizeof *graph);
    if (graph == NULL) return NULL;

    graph->vertex_count = vertex_count;
    graph->directed = directed;
    return graph;
}

void int_graph_free(IntGraph *graph) {
    if (graph == NULL) return;
    free(graph->edges);
    free(graph);
}

size_t int_graph_vertex_count(const IntGraph *graph) {
    return graph == NULL ? 0 : graph->vertex_count;
}

size_t int_graph_edge_count(const IntGraph *graph) {
    return graph == NULL ? 0 : graph->edge_count;
}

bool int_graph_is_directed(const IntGraph *graph) {
    return graph != NULL && graph->directed;
}

static bool reserve(IntGraph *graph, size_t needed) {
    if (needed <= graph->capacity) return true;

    size_t capacity = graph->capacity == 0 ? 8 : graph->capacity;

    while (capacity < needed) {
        if (capacity > SIZE_MAX / 2) {
            capacity = needed;
            break;
        }
        capacity *= 2;
    }

    if (capacity > SIZE_MAX / sizeof *graph->edges) return false;

    IntGraphEdge *next = realloc(
        graph->edges,
        capacity * sizeof *next
    );
    if (next == NULL) return false;

    graph->edges = next;
    graph->capacity = capacity;
    return true;
}

static size_t find_edge_index(
    const IntGraph *graph,
    size_t from,
    size_t to
) {
    for (size_t i = 0; i < graph->edge_count; ++i) {
        if (graph->edges[i].from == from &&
            graph->edges[i].to == to) {
            return i;
        }
    }

    return SIZE_MAX;
}

bool int_graph_add_edge(
    IntGraph *graph,
    size_t from,
    size_t to,
    int weight
) {
    if (graph == NULL) return false;
    if (from >= graph->vertex_count || to >= graph->vertex_count) {
        return false;
    }
    if (from == to) return false;

    canonicalize(graph, &from, &to);

    if (find_edge_index(graph, from, to) != SIZE_MAX) {
        return false;
    }

    if (graph->edge_count == SIZE_MAX) return false;
    if (!reserve(graph, graph->edge_count + 1)) return false;

    graph->edges[graph->edge_count++] = (IntGraphEdge){
        .from = from,
        .to = to,
        .weight = weight
    };

    return true;
}

bool int_graph_remove_edge(
    IntGraph *graph,
    size_t from,
    size_t to
) {
    if (graph == NULL) return false;
    if (from >= graph->vertex_count || to >= graph->vertex_count) {
        return false;
    }

    canonicalize(graph, &from, &to);

    const size_t index = find_edge_index(graph, from, to);
    if (index == SIZE_MAX) return false;

    for (size_t i = index + 1; i < graph->edge_count; ++i) {
        graph->edges[i - 1] = graph->edges[i];
    }

    --graph->edge_count;
    return true;
}

bool int_graph_has_edge(
    const IntGraph *graph,
    size_t from,
    size_t to,
    int *out_weight
) {
    if (graph == NULL) return false;
    if (from >= graph->vertex_count || to >= graph->vertex_count) {
        return false;
    }

    canonicalize(graph, &from, &to);

    const size_t index = find_edge_index(graph, from, to);
    if (index == SIZE_MAX) return false;

    if (out_weight != NULL) {
        *out_weight = graph->edges[index].weight;
    }

    return true;
}

bool int_graph_edge_at(
    const IntGraph *graph,
    size_t index,
    IntGraphEdge *out_edge
) {
    if (graph == NULL || out_edge == NULL ||
        index >= graph->edge_count) {
        return false;
    }

    *out_edge = graph->edges[index];
    return true;
}

bool int_graph_degree(
    const IntGraph *graph,
    size_t vertex,
    size_t *out_degree
) {
    if (graph == NULL || out_degree == NULL ||
        vertex >= graph->vertex_count || graph->directed) {
        return false;
    }

    size_t degree = 0;

    for (size_t i = 0; i < graph->edge_count; ++i) {
        if (graph->edges[i].from == vertex ||
            graph->edges[i].to == vertex) {
            ++degree;
        }
    }

    *out_degree = degree;
    return true;
}

bool int_graph_in_degree(
    const IntGraph *graph,
    size_t vertex,
    size_t *out_degree
) {
    if (graph == NULL || out_degree == NULL ||
        vertex >= graph->vertex_count || !graph->directed) {
        return false;
    }

    size_t degree = 0;

    for (size_t i = 0; i < graph->edge_count; ++i) {
        if (graph->edges[i].to == vertex) ++degree;
    }

    *out_degree = degree;
    return true;
}

bool int_graph_out_degree(
    const IntGraph *graph,
    size_t vertex,
    size_t *out_degree
) {
    if (graph == NULL || out_degree == NULL ||
        vertex >= graph->vertex_count || !graph->directed) {
        return false;
    }

    size_t degree = 0;

    for (size_t i = 0; i < graph->edge_count; ++i) {
        if (graph->edges[i].from == vertex) ++degree;
    }

    *out_degree = degree;
    return true;
}

bool int_graph_validate(const IntGraph *graph) {
    if (graph == NULL) return false;
    if (graph->edge_count > graph->capacity) return false;
    if (graph->capacity > 0 && graph->edges == NULL) return false;

    for (size_t i = 0; i < graph->edge_count; ++i) {
        const IntGraphEdge edge = graph->edges[i];

        if (edge.from >= graph->vertex_count ||
            edge.to >= graph->vertex_count) {
            return false;
        }

        if (edge.from == edge.to) return false;

        if (!graph->directed && edge.from >= edge.to) {
            return false;
        }

        for (size_t j = i + 1; j < graph->edge_count; ++j) {
            if (edge.from == graph->edges[j].from &&
                edge.to == graph->edges[j].to) {
                return false;
            }
        }
    }

    return true;
}
