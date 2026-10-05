#include "graph_traversal.h"

#include <stdint.h>
#include <stdlib.h>

struct GraphTraversal {
    size_t vertex_count;
    bool *visited;
    size_t *parent;
    size_t *depth;
    size_t *order;
    size_t order_count;

    size_t *queue;
    size_t *stack;
    size_t *scratch;
    size_t scratch_count;

    size_t source;
    GraphTraversalMode mode;
    bool has_run;
};

GraphTraversal *graph_traversal_create(size_t vertex_count) {
    GraphTraversal *t = calloc(1, sizeof *t);
    if (t == NULL) return NULL;

    t->vertex_count = vertex_count;

    if (vertex_count == 0) return t;

    if (vertex_count > SIZE_MAX / sizeof *t->visited ||
        vertex_count > SIZE_MAX / sizeof *t->parent ||
        vertex_count > SIZE_MAX / sizeof *t->depth ||
        vertex_count > SIZE_MAX / sizeof *t->order) {
        free(t);
        return NULL;
    }

    t->visited = calloc(vertex_count, sizeof *t->visited);
    t->parent = malloc(vertex_count * sizeof *t->parent);
    t->depth = malloc(vertex_count * sizeof *t->depth);
    t->order = malloc(vertex_count * sizeof *t->order);
    t->queue = malloc(vertex_count * sizeof *t->queue);
    t->stack = malloc(vertex_count * sizeof *t->stack);
    t->scratch = malloc(vertex_count * sizeof *t->scratch);

    if (t->visited == NULL ||
        t->parent == NULL ||
        t->depth == NULL ||
        t->order == NULL ||
        t->queue == NULL ||
        t->stack == NULL ||
        t->scratch == NULL) {
        graph_traversal_free(t);
        return NULL;
    }

    return t;
}

void graph_traversal_free(GraphTraversal *t) {
    if (t == NULL) return;

    free(t->visited);
    free(t->parent);
    free(t->depth);
    free(t->order);
    free(t->queue);
    free(t->stack);
    free(t->scratch);
    free(t);
}

static void reset(GraphTraversal *t) {
    for (size_t i = 0; i < t->vertex_count; ++i) {
        t->visited[i] = false;
        t->parent[i] = SIZE_MAX;
        t->depth[i] = SIZE_MAX;
    }

    t->order_count = 0;
    t->scratch_count = 0;
    t->source = SIZE_MAX;
    t->mode = GRAPH_TRAVERSAL_NONE;
    t->has_run = false;
}

typedef struct {
    GraphTraversal *traversal;
    bool ok;
} NeighborCollector;

static bool collect_neighbor(
    size_t neighbor,
    int weight,
    void *context
) {
    NeighborCollector *collector = context;
    GraphTraversal *t = collector->traversal;
    (void)weight;

    if (t->scratch_count >= t->vertex_count) {
        collector->ok = false;
        return false;
    }

    t->scratch[t->scratch_count++] = neighbor;
    return true;
}

static bool load_neighbors(
    GraphTraversal *t,
    const GraphRepr *graph,
    size_t vertex
) {
    t->scratch_count = 0;

    NeighborCollector collector = {
        .traversal = t,
        .ok = true
    };

    if (!graph_repr_for_each_neighbor(
            graph,
            vertex,
            collect_neighbor,
            &collector
        )) {
        return collector.ok;
    }

    return collector.ok;
}

static bool prepare(
    GraphTraversal *t,
    const GraphRepr *graph,
    size_t source,
    GraphTraversalMode mode
) {
    if (t == NULL || graph == NULL) return false;
    if (t->vertex_count != graph_repr_vertex_count(graph)) return false;
    if (source >= t->vertex_count) return false;

    reset(t);

    t->source = source;
    t->mode = mode;
    t->has_run = true;
    t->visited[source] = true;
    t->depth[source] = 0;

    return true;
}

bool graph_traversal_bfs(
    GraphTraversal *t,
    const GraphRepr *graph,
    size_t source
) {
    if (!prepare(t, graph, source, GRAPH_TRAVERSAL_BFS)) {
        return false;
    }

    size_t head = 0;
    size_t tail = 0;

    t->queue[tail++] = source;

    while (head < tail) {
        const size_t u = t->queue[head++];
        t->order[t->order_count++] = u;

        if (!load_neighbors(t, graph, u)) return false;

        for (size_t i = 0; i < t->scratch_count; ++i) {
            const size_t v = t->scratch[i];

            if (v >= t->vertex_count) return false;

            if (!t->visited[v]) {
                if (tail >= t->vertex_count) return false;

                t->visited[v] = true;
                t->parent[v] = u;

                if (t->depth[u] == SIZE_MAX ||
                    t->depth[u] == SIZE_MAX - 1) {
                    return false;
                }

                t->depth[v] = t->depth[u] + 1;
                t->queue[tail++] = v;
            }
        }
    }

    return true;
}

bool graph_traversal_dfs(
    GraphTraversal *t,
    const GraphRepr *graph,
    size_t source
) {
    if (!prepare(t, graph, source, GRAPH_TRAVERSAL_DFS)) {
        return false;
    }

    size_t top = 0;
    t->stack[top++] = source;

    while (top > 0) {
        const size_t u = t->stack[--top];
        t->order[t->order_count++] = u;

        if (!load_neighbors(t, graph, u)) return false;

        for (size_t i = t->scratch_count; i > 0; --i) {
            const size_t v = t->scratch[i - 1];

            if (v >= t->vertex_count) return false;

            if (!t->visited[v]) {
                if (top >= t->vertex_count) return false;

                t->visited[v] = true;
                t->parent[v] = u;

                if (t->depth[u] == SIZE_MAX ||
                    t->depth[u] == SIZE_MAX - 1) {
                    return false;
                }

                t->depth[v] = t->depth[u] + 1;
                t->stack[top++] = v;
            }
        }
    }

    return true;
}

size_t graph_traversal_order_count(const GraphTraversal *t) {
    return t == NULL ? 0 : t->order_count;
}

bool graph_traversal_order_at(
    const GraphTraversal *t,
    size_t index,
    size_t *out_vertex
) {
    if (t == NULL || out_vertex == NULL ||
        index >= t->order_count) {
        return false;
    }

    *out_vertex = t->order[index];
    return true;
}

bool graph_traversal_visited(
    const GraphTraversal *t,
    size_t vertex,
    bool *out_visited
) {
    if (t == NULL || out_visited == NULL ||
        vertex >= t->vertex_count) {
        return false;
    }

    *out_visited = t->visited[vertex];
    return true;
}

bool graph_traversal_parent(
    const GraphTraversal *t,
    size_t vertex,
    size_t *out_parent
) {
    if (t == NULL || out_parent == NULL ||
        vertex >= t->vertex_count ||
        !t->has_run ||
        !t->visited[vertex]) {
        return false;
    }

    *out_parent = t->parent[vertex];
    return true;
}

bool graph_traversal_depth(
    const GraphTraversal *t,
    size_t vertex,
    size_t *out_depth
) {
    if (t == NULL || out_depth == NULL ||
        vertex >= t->vertex_count ||
        !t->has_run ||
        !t->visited[vertex]) {
        return false;
    }

    *out_depth = t->depth[vertex];
    return true;
}

bool graph_traversal_path_to(
    const GraphTraversal *t,
    size_t target,
    size_t *output,
    size_t capacity,
    size_t *out_written
) {
    if (t == NULL || out_written == NULL ||
        target >= t->vertex_count ||
        !t->has_run ||
        !t->visited[target]) {
        return false;
    }

    size_t length = 0;
    size_t cursor = target;

    for (;;) {
        ++length;

        if (cursor == t->source) break;

        cursor = t->parent[cursor];

        if (cursor == SIZE_MAX ||
            cursor >= t->vertex_count ||
            length > t->vertex_count) {
            return false;
        }
    }

    if (output == NULL || capacity < length) return false;

    cursor = target;

    for (size_t i = length; i > 0; --i) {
        output[i - 1] = cursor;

        if (cursor == t->source) break;
        cursor = t->parent[cursor];
    }

    *out_written = length;
    return true;
}

GraphTraversalMode graph_traversal_mode(
    const GraphTraversal *t
) {
    return t == NULL ? GRAPH_TRAVERSAL_NONE : t->mode;
}

bool graph_traversal_validate(
    const GraphTraversal *t,
    const GraphRepr *graph
) {
    if (t == NULL || graph == NULL) return false;
    if (!t->has_run) return false;
    if (t->vertex_count != graph_repr_vertex_count(graph)) return false;
    if (t->source >= t->vertex_count) return false;
    if (t->order_count > t->vertex_count) return false;

    bool *seen_in_order = calloc(
        t->vertex_count == 0 ? 1 : t->vertex_count,
        sizeof *seen_in_order
    );
    if (seen_in_order == NULL) return false;

    size_t visited_count = 0;

    for (size_t i = 0; i < t->order_count; ++i) {
        const size_t v = t->order[i];

        if (v >= t->vertex_count ||
            seen_in_order[v] ||
            !t->visited[v]) {
            free(seen_in_order);
            return false;
        }

        seen_in_order[v] = true;
    }

    for (size_t v = 0; v < t->vertex_count; ++v) {
        if (!t->visited[v]) {
            if (t->parent[v] != SIZE_MAX ||
                t->depth[v] != SIZE_MAX) {
                free(seen_in_order);
                return false;
            }
            continue;
        }

        ++visited_count;

        if (!seen_in_order[v]) {
            free(seen_in_order);
            return false;
        }

        if (v == t->source) {
            if (t->parent[v] != SIZE_MAX ||
                t->depth[v] != 0) {
                free(seen_in_order);
                return false;
            }
        } else {
            const size_t p = t->parent[v];

            if (p >= t->vertex_count ||
                !t->visited[p] ||
                t->depth[p] == SIZE_MAX ||
                t->depth[v] != t->depth[p] + 1 ||
                !graph_repr_has_edge(graph, p, v, NULL)) {
                free(seen_in_order);
                return false;
            }
        }
    }

    free(seen_in_order);
    return visited_count == t->order_count;
}
