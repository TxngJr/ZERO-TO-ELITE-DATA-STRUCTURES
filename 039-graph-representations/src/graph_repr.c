#include "graph_repr.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t from;
    size_t to;
    int weight;
} Edge;

typedef struct {
    size_t to;
    int weight;
} Neighbor;

typedef struct {
    Neighbor *items;
    size_t count;
    size_t capacity;
} NeighborVec;

struct GraphRepr {
    size_t vertex_count;
    size_t edge_count;
    bool directed;
    GraphReprKind kind;

    Edge *edges;
    size_t edge_capacity;

    unsigned char *present;
    int *matrix_weights;

    NeighborVec *adj;
};

static bool valid_kind(GraphReprKind kind) {
    return kind == GRAPH_REPR_EDGE_LIST ||
           kind == GRAPH_REPR_ADJ_MATRIX ||
           kind == GRAPH_REPR_ADJ_LIST;
}

static void canonicalize(
    const GraphRepr *graph,
    size_t *from,
    size_t *to
) {
    if (!graph->directed && *from > *to) {
        const size_t tmp = *from;
        *from = *to;
        *to = tmp;
    }
}

GraphRepr *graph_repr_create(
    size_t vertex_count,
    bool directed,
    GraphReprKind kind
) {
    if (!valid_kind(kind)) return NULL;

    GraphRepr *graph = calloc(1, sizeof *graph);
    if (graph == NULL) return NULL;

    graph->vertex_count = vertex_count;
    graph->directed = directed;
    graph->kind = kind;

    if (kind == GRAPH_REPR_ADJ_MATRIX && vertex_count > 0) {
        if (vertex_count > SIZE_MAX / vertex_count) {
            free(graph);
            return NULL;
        }

        const size_t cells = vertex_count * vertex_count;

        if (cells > SIZE_MAX / sizeof *graph->matrix_weights) {
            free(graph);
            return NULL;
        }

        graph->present = calloc(cells, sizeof *graph->present);
        graph->matrix_weights = malloc(
            cells * sizeof *graph->matrix_weights
        );

        if (graph->present == NULL ||
            graph->matrix_weights == NULL) {
            free(graph->present);
            free(graph->matrix_weights);
            free(graph);
            return NULL;
        }
    }

    if (kind == GRAPH_REPR_ADJ_LIST && vertex_count > 0) {
        if (vertex_count > SIZE_MAX / sizeof *graph->adj) {
            free(graph);
            return NULL;
        }

        graph->adj = calloc(vertex_count, sizeof *graph->adj);

        if (graph->adj == NULL) {
            free(graph);
            return NULL;
        }
    }

    return graph;
}

void graph_repr_free(GraphRepr *graph) {
    if (graph == NULL) return;

    free(graph->edges);
    free(graph->present);
    free(graph->matrix_weights);

    if (graph->adj != NULL) {
        for (size_t i = 0; i < graph->vertex_count; ++i) {
            free(graph->adj[i].items);
        }
    }

    free(graph->adj);
    free(graph);
}

size_t graph_repr_vertex_count(const GraphRepr *graph) {
    return graph == NULL ? 0 : graph->vertex_count;
}

size_t graph_repr_edge_count(const GraphRepr *graph) {
    return graph == NULL ? 0 : graph->edge_count;
}

GraphReprKind graph_repr_kind(const GraphRepr *graph) {
    return graph == NULL ? 0 : graph->kind;
}

static bool reserve_edges(GraphRepr *graph, size_t needed) {
    if (needed <= graph->edge_capacity) return true;

    size_t capacity = graph->edge_capacity == 0 ? 8 : graph->edge_capacity;

    while (capacity < needed) {
        if (capacity > SIZE_MAX / 2) {
            capacity = needed;
            break;
        }
        capacity *= 2;
    }

    if (capacity > SIZE_MAX / sizeof *graph->edges) return false;

    Edge *next = realloc(graph->edges, capacity * sizeof *next);
    if (next == NULL) return false;

    graph->edges = next;
    graph->edge_capacity = capacity;
    return true;
}

static size_t edge_list_find(
    const GraphRepr *graph,
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

static size_t matrix_index(
    const GraphRepr *graph,
    size_t from,
    size_t to
) {
    return from * graph->vertex_count + to;
}

static size_t neighbor_lower_bound(
    const NeighborVec *vec,
    size_t to
) {
    size_t low = 0;
    size_t high = vec->count;

    while (low < high) {
        const size_t mid = low + (high - low) / 2;

        if (vec->items[mid].to < to) low = mid + 1;
        else high = mid;
    }

    return low;
}

static bool reserve_neighbors(
    NeighborVec *vec,
    size_t needed
) {
    if (needed <= vec->capacity) return true;

    size_t capacity = vec->capacity == 0 ? 4 : vec->capacity;

    while (capacity < needed) {
        if (capacity > SIZE_MAX / 2) {
            capacity = needed;
            break;
        }
        capacity *= 2;
    }

    if (capacity > SIZE_MAX / sizeof *vec->items) return false;

    Neighbor *next = realloc(vec->items, capacity * sizeof *next);
    if (next == NULL) return false;

    vec->items = next;
    vec->capacity = capacity;
    return true;
}

static bool list_insert_one(
    NeighborVec *vec,
    size_t to,
    int weight
) {
    const size_t pos = neighbor_lower_bound(vec, to);

    if (pos < vec->count && vec->items[pos].to == to) {
        return false;
    }

    if (!reserve_neighbors(vec, vec->count + 1)) return false;

    for (size_t i = vec->count; i > pos; --i) {
        vec->items[i] = vec->items[i - 1];
    }

    vec->items[pos] = (Neighbor){.to = to, .weight = weight};
    ++vec->count;
    return true;
}

static bool list_remove_one(
    NeighborVec *vec,
    size_t to
) {
    const size_t pos = neighbor_lower_bound(vec, to);

    if (pos >= vec->count || vec->items[pos].to != to) {
        return false;
    }

    for (size_t i = pos + 1; i < vec->count; ++i) {
        vec->items[i - 1] = vec->items[i];
    }

    --vec->count;
    return true;
}

bool graph_repr_has_edge(
    const GraphRepr *graph,
    size_t from,
    size_t to,
    int *out_weight
) {
    if (graph == NULL ||
        from >= graph->vertex_count ||
        to >= graph->vertex_count) {
        return false;
    }

    if (graph->kind == GRAPH_REPR_EDGE_LIST) {
        canonicalize(graph, &from, &to);
        const size_t index = edge_list_find(graph, from, to);

        if (index == SIZE_MAX) return false;

        if (out_weight != NULL) {
            *out_weight = graph->edges[index].weight;
        }
        return true;
    }

    if (graph->kind == GRAPH_REPR_ADJ_MATRIX) {
        const size_t index = matrix_index(graph, from, to);

        if (!graph->present[index]) return false;

        if (out_weight != NULL) {
            *out_weight = graph->matrix_weights[index];
        }
        return true;
    }

    const NeighborVec *vec = &graph->adj[from];
    const size_t pos = neighbor_lower_bound(vec, to);

    if (pos >= vec->count || vec->items[pos].to != to) {
        return false;
    }

    if (out_weight != NULL) {
        *out_weight = vec->items[pos].weight;
    }

    return true;
}

bool graph_repr_add_edge(
    GraphRepr *graph,
    size_t from,
    size_t to,
    int weight
) {
    if (graph == NULL ||
        from >= graph->vertex_count ||
        to >= graph->vertex_count ||
        from == to ||
        graph->edge_count == SIZE_MAX) {
        return false;
    }

    if (graph_repr_has_edge(graph, from, to, NULL)) return false;

    if (graph->kind == GRAPH_REPR_EDGE_LIST) {
        canonicalize(graph, &from, &to);

        if (!reserve_edges(graph, graph->edge_count + 1)) {
            return false;
        }

        graph->edges[graph->edge_count++] = (Edge){
            .from = from,
            .to = to,
            .weight = weight
        };

        return true;
    }

    if (graph->kind == GRAPH_REPR_ADJ_MATRIX) {
        const size_t a = matrix_index(graph, from, to);
        graph->present[a] = 1;
        graph->matrix_weights[a] = weight;

        if (!graph->directed) {
            const size_t b = matrix_index(graph, to, from);
            graph->present[b] = 1;
            graph->matrix_weights[b] = weight;
        }

        ++graph->edge_count;
        return true;
    }

    if (!reserve_neighbors(
            &graph->adj[from],
            graph->adj[from].count + 1
        )) {
        return false;
    }

    if (!graph->directed &&
        !reserve_neighbors(
            &graph->adj[to],
            graph->adj[to].count + 1
        )) {
        return false;
    }

    if (!list_insert_one(&graph->adj[from], to, weight)) {
        return false;
    }

    if (!graph->directed &&
        !list_insert_one(&graph->adj[to], from, weight)) {
        (void)list_remove_one(&graph->adj[from], to);
        return false;
    }

    ++graph->edge_count;
    return true;
}

bool graph_repr_remove_edge(
    GraphRepr *graph,
    size_t from,
    size_t to
) {
    if (graph == NULL ||
        from >= graph->vertex_count ||
        to >= graph->vertex_count) {
        return false;
    }

    if (!graph_repr_has_edge(graph, from, to, NULL)) return false;

    if (graph->kind == GRAPH_REPR_EDGE_LIST) {
        canonicalize(graph, &from, &to);
        const size_t index = edge_list_find(graph, from, to);

        for (size_t i = index + 1; i < graph->edge_count; ++i) {
            graph->edges[i - 1] = graph->edges[i];
        }

        --graph->edge_count;
        return true;
    }

    if (graph->kind == GRAPH_REPR_ADJ_MATRIX) {
        graph->present[matrix_index(graph, from, to)] = 0;

        if (!graph->directed) {
            graph->present[matrix_index(graph, to, from)] = 0;
        }

        --graph->edge_count;
        return true;
    }

    if (!list_remove_one(&graph->adj[from], to)) return false;

    if (!graph->directed &&
        !list_remove_one(&graph->adj[to], from)) {
        return false;
    }

    --graph->edge_count;
    return true;
}

bool graph_repr_for_each_neighbor(
    const GraphRepr *graph,
    size_t vertex,
    graph_neighbor_visit_fn visitor,
    void *context
) {
    if (graph == NULL || visitor == NULL ||
        vertex >= graph->vertex_count) {
        return false;
    }

    if (graph->kind == GRAPH_REPR_EDGE_LIST) {
        for (size_t i = 0; i < graph->edge_count; ++i) {
            const Edge edge = graph->edges[i];

            if (graph->directed) {
                if (edge.from == vertex &&
                    !visitor(edge.to, edge.weight, context)) {
                    return false;
                }
            } else if (edge.from == vertex) {
                if (!visitor(edge.to, edge.weight, context)) return false;
            } else if (edge.to == vertex) {
                if (!visitor(edge.from, edge.weight, context)) return false;
            }
        }

        return true;
    }

    if (graph->kind == GRAPH_REPR_ADJ_MATRIX) {
        for (size_t to = 0; to < graph->vertex_count; ++to) {
            const size_t index = matrix_index(graph, vertex, to);

            if (graph->present[index] &&
                !visitor(to, graph->matrix_weights[index], context)) {
                return false;
            }
        }

        return true;
    }

    const NeighborVec *vec = &graph->adj[vertex];

    for (size_t i = 0; i < vec->count; ++i) {
        if (!visitor(
                vec->items[i].to,
                vec->items[i].weight,
                context
            )) {
            return false;
        }
    }

    return true;
}

size_t graph_repr_memory_bytes(const GraphRepr *graph) {
    if (graph == NULL) return 0;

    size_t bytes = sizeof *graph;

    if (graph->kind == GRAPH_REPR_EDGE_LIST) {
        if (graph->edge_capacity >
            (SIZE_MAX - bytes) / sizeof *graph->edges) {
            return SIZE_MAX;
        }

        return bytes +
               graph->edge_capacity * sizeof *graph->edges;
    }

    if (graph->kind == GRAPH_REPR_ADJ_MATRIX) {
        const size_t cells =
            graph->vertex_count * graph->vertex_count;
        const size_t per_cell =
            sizeof(unsigned char) + sizeof(int);

        if (cells > SIZE_MAX / per_cell) return SIZE_MAX;
        if (bytes > SIZE_MAX - cells * per_cell) return SIZE_MAX;

        return bytes + cells * per_cell;
    }

    if (graph->vertex_count >
        (SIZE_MAX - bytes) / sizeof *graph->adj) {
        return SIZE_MAX;
    }

    bytes += graph->vertex_count * sizeof *graph->adj;

    for (size_t i = 0; i < graph->vertex_count; ++i) {
        if (graph->adj[i].capacity >
            (SIZE_MAX - bytes) / sizeof *graph->adj[i].items) {
            return SIZE_MAX;
        }

        bytes += graph->adj[i].capacity *
                 sizeof *graph->adj[i].items;
    }

    return bytes;
}

bool graph_repr_validate(const GraphRepr *graph) {
    if (graph == NULL || !valid_kind(graph->kind)) return false;

    if (graph->kind == GRAPH_REPR_EDGE_LIST) {
        if (graph->edge_count > graph->edge_capacity) return false;
        if (graph->edge_capacity > 0 && graph->edges == NULL) return false;

        for (size_t i = 0; i < graph->edge_count; ++i) {
            const Edge edge = graph->edges[i];

            if (edge.from >= graph->vertex_count ||
                edge.to >= graph->vertex_count ||
                edge.from == edge.to) {
                return false;
            }

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

    if (graph->kind == GRAPH_REPR_ADJ_MATRIX) {
        size_t logical_edges = 0;

        for (size_t u = 0; u < graph->vertex_count; ++u) {
            if (graph->present[matrix_index(graph,u,u)]) return false;

            for (size_t v = 0; v < graph->vertex_count; ++v) {
                const size_t a = matrix_index(graph,u,v);

                if (!graph->present[a]) continue;

                if (graph->directed) {
                    ++logical_edges;
                } else {
                    const size_t b = matrix_index(graph,v,u);

                    if (!graph->present[b] ||
                        graph->matrix_weights[a] !=
                        graph->matrix_weights[b]) {
                        return false;
                    }

                    if (u < v) ++logical_edges;
                }
            }
        }

        return logical_edges == graph->edge_count;
    }

    size_t arcs = 0;

    for (size_t u = 0; u < graph->vertex_count; ++u) {
        const NeighborVec *vec = &graph->adj[u];

        if (vec->count > vec->capacity) return false;
        if (vec->capacity > 0 && vec->items == NULL) return false;

        for (size_t i = 0; i < vec->count; ++i) {
            if (vec->items[i].to >= graph->vertex_count ||
                vec->items[i].to == u) {
                return false;
            }

            if (i > 0 &&
                vec->items[i - 1].to >= vec->items[i].to) {
                return false;
            }

            if (!graph->directed) {
                int reverse_weight = 0;

                if (!graph_repr_has_edge(
                        graph,
                        vec->items[i].to,
                        u,
                        &reverse_weight
                    ) ||
                    reverse_weight != vec->items[i].weight) {
                    return false;
                }
            }

            ++arcs;
        }
    }

    if (graph->directed) {
        return arcs == graph->edge_count;
    }

    if (graph->edge_count > SIZE_MAX / 2) return false;
    return arcs == graph->edge_count * 2;
}
