#include "adaptive_graph.h"

#include <stdint.h>
#include <stdlib.h>

struct AdaptiveGraph {
    size_t vertex_count;
    bool directed;
    unsigned promote_percent;
    unsigned demote_percent;
    GraphRepr *backend;
    size_t switch_count;
};

static long double maximum_edges(
    const AdaptiveGraph *graph
) {
    if (graph == NULL || graph->vertex_count < 2) {
        return 0.0L;
    }

    const long double v = (long double)graph->vertex_count;

    if (graph->directed) {
        return v * (v - 1.0L);
    }

    return v * (v - 1.0L) / 2.0L;
}

AdaptiveGraph *adaptive_graph_create(
    size_t vertex_count,
    bool directed,
    unsigned promote_percent,
    unsigned demote_percent
) {
    if (promote_percent == 0 ||
        promote_percent > 100 ||
        demote_percent >= promote_percent) {
        return NULL;
    }

    AdaptiveGraph *graph = calloc(1, sizeof *graph);
    if (graph == NULL) return NULL;

    graph->vertex_count = vertex_count;
    graph->directed = directed;
    graph->promote_percent = promote_percent;
    graph->demote_percent = demote_percent;

    graph->backend = graph_repr_create(
        vertex_count,
        directed,
        GRAPH_REPR_ADJ_LIST
    );

    if (graph->backend == NULL) {
        free(graph);
        return NULL;
    }

    return graph;
}

void adaptive_graph_free(AdaptiveGraph *graph) {
    if (graph == NULL) return;
    graph_repr_free(graph->backend);
    free(graph);
}

size_t adaptive_graph_vertex_count(
    const AdaptiveGraph *graph
) {
    return graph == NULL ? 0 : graph->vertex_count;
}

size_t adaptive_graph_edge_count(
    const AdaptiveGraph *graph
) {
    return graph == NULL
        ? 0
        : graph_repr_edge_count(graph->backend);
}

GraphReprKind adaptive_graph_backend_kind(
    const AdaptiveGraph *graph
) {
    return graph == NULL
        ? 0
        : graph_repr_kind(graph->backend);
}

size_t adaptive_graph_switch_count(
    const AdaptiveGraph *graph
) {
    return graph == NULL ? 0 : graph->switch_count;
}

double adaptive_graph_density_percent(
    const AdaptiveGraph *graph
) {
    if (graph == NULL) return 0.0;

    const long double maximum = maximum_edges(graph);

    if (maximum == 0.0L) return 0.0;

    const long double edges =
        (long double)graph_repr_edge_count(graph->backend);

    return (double)(100.0L * edges / maximum);
}

size_t adaptive_graph_memory_bytes(
    const AdaptiveGraph *graph
) {
    if (graph == NULL) return 0;

    const size_t backend =
        graph_repr_memory_bytes(graph->backend);

    if (backend == SIZE_MAX ||
        sizeof *graph > SIZE_MAX - backend) {
        return SIZE_MAX;
    }

    return sizeof *graph + backend;
}

typedef struct {
    GraphRepr *target;
    size_t from;
    bool directed;
    bool ok;
} ReplayContext;

static bool replay_neighbor(
    size_t neighbor,
    int weight,
    void *context
) {
    ReplayContext *replay = context;

    if (!replay->directed && replay->from > neighbor) {
        return true;
    }

    if (!graph_repr_add_edge(
            replay->target,
            replay->from,
            neighbor,
            weight
        )) {
        replay->ok = false;
        return false;
    }

    return true;
}

static bool convert_to(
    AdaptiveGraph *graph,
    GraphReprKind target_kind
) {
    if (graph == NULL) return false;

    const GraphReprKind current =
        graph_repr_kind(graph->backend);

    if (current == target_kind) return true;

    if (target_kind != GRAPH_REPR_ADJ_LIST &&
        target_kind != GRAPH_REPR_ADJ_MATRIX) {
        return false;
    }

    GraphRepr *next = graph_repr_create(
        graph->vertex_count,
        graph->directed,
        target_kind
    );

    if (next == NULL) return false;

    for (size_t u = 0; u < graph->vertex_count; ++u) {
        ReplayContext replay = {
            .target = next,
            .from = u,
            .directed = graph->directed,
            .ok = true
        };

        if (!graph_repr_for_each_neighbor(
                graph->backend,
                u,
                replay_neighbor,
                &replay
            ) ||
            !replay.ok) {
            graph_repr_free(next);
            return false;
        }
    }

    if (graph_repr_edge_count(next) !=
            graph_repr_edge_count(graph->backend) ||
        !graph_repr_validate(next)) {
        graph_repr_free(next);
        return false;
    }

    GraphRepr *old = graph->backend;
    graph->backend = next;
    graph_repr_free(old);

    if (graph->switch_count != SIZE_MAX) {
        ++graph->switch_count;
    }

    return true;
}

bool adaptive_graph_rebalance(AdaptiveGraph *graph) {
    if (graph == NULL || graph->backend == NULL) {
        return false;
    }

    const long double maximum = maximum_edges(graph);

    if (maximum == 0.0L) return true;

    const long double density =
        100.0L *
        (long double)graph_repr_edge_count(graph->backend) /
        maximum;

    const GraphReprKind kind =
        graph_repr_kind(graph->backend);

    if (kind == GRAPH_REPR_ADJ_LIST &&
        density >= (long double)graph->promote_percent) {
        return convert_to(
            graph,
            GRAPH_REPR_ADJ_MATRIX
        );
    }

    if (kind == GRAPH_REPR_ADJ_MATRIX &&
        density <= (long double)graph->demote_percent) {
        return convert_to(
            graph,
            GRAPH_REPR_ADJ_LIST
        );
    }

    return true;
}

bool adaptive_graph_add_edge(
    AdaptiveGraph *graph,
    size_t from,
    size_t to,
    int weight
) {
    if (graph == NULL) return false;

    if (!graph_repr_add_edge(
            graph->backend,
            from,
            to,
            weight
        )) {
        return false;
    }

    (void)adaptive_graph_rebalance(graph);
    return true;
}

bool adaptive_graph_remove_edge(
    AdaptiveGraph *graph,
    size_t from,
    size_t to
) {
    if (graph == NULL) return false;

    if (!graph_repr_remove_edge(
            graph->backend,
            from,
            to
        )) {
        return false;
    }

    (void)adaptive_graph_rebalance(graph);
    return true;
}

bool adaptive_graph_has_edge(
    const AdaptiveGraph *graph,
    size_t from,
    size_t to,
    int *out_weight
) {
    if (graph == NULL) return false;

    return graph_repr_has_edge(
        graph->backend,
        from,
        to,
        out_weight
    );
}

bool adaptive_graph_for_each_neighbor(
    const AdaptiveGraph *graph,
    size_t vertex,
    graph_neighbor_visit_fn visitor,
    void *context
) {
    if (graph == NULL) return false;

    return graph_repr_for_each_neighbor(
        graph->backend,
        vertex,
        visitor,
        context
    );
}

bool adaptive_graph_validate(
    const AdaptiveGraph *graph
) {
    if (graph == NULL || graph->backend == NULL) {
        return false;
    }

    if (graph->promote_percent == 0 ||
        graph->promote_percent > 100 ||
        graph->demote_percent >= graph->promote_percent) {
        return false;
    }

    if (graph_repr_vertex_count(graph->backend) !=
        graph->vertex_count) {
        return false;
    }

    const GraphReprKind kind =
        graph_repr_kind(graph->backend);

    if (kind != GRAPH_REPR_ADJ_LIST &&
        kind != GRAPH_REPR_ADJ_MATRIX) {
        return false;
    }

    return graph_repr_validate(graph->backend);
}
