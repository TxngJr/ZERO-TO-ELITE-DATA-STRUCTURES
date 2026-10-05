#include "int_dag.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t *items;
    size_t count;
    size_t capacity;
} SizeVec;

struct IntDAG {
    size_t vertex_count;
    size_t edge_count;
    SizeVec *out;
    size_t *indegree;
};

static size_t lower_bound(
    const SizeVec *vec,
    size_t value
) {
    size_t low = 0;
    size_t high = vec->count;

    while (low < high) {
        const size_t mid = low + (high - low) / 2;

        if (vec->items[mid] < value) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    return low;
}

static bool reserve_vec(
    SizeVec *vec,
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

    size_t *next = realloc(
        vec->items,
        capacity * sizeof *next
    );
    if (next == NULL) return false;

    vec->items = next;
    vec->capacity = capacity;
    return true;
}

IntDAG *int_dag_create(size_t vertex_count) {
    IntDAG *dag = calloc(1, sizeof *dag);
    if (dag == NULL) return NULL;

    dag->vertex_count = vertex_count;

    if (vertex_count == 0) return dag;

    if (vertex_count > SIZE_MAX / sizeof *dag->out ||
        vertex_count > SIZE_MAX / sizeof *dag->indegree) {
        free(dag);
        return NULL;
    }

    dag->out = calloc(vertex_count, sizeof *dag->out);
    dag->indegree = calloc(
        vertex_count,
        sizeof *dag->indegree
    );

    if (dag->out == NULL || dag->indegree == NULL) {
        free(dag->out);
        free(dag->indegree);
        free(dag);
        return NULL;
    }

    return dag;
}

void int_dag_free(IntDAG *dag) {
    if (dag == NULL) return;

    if (dag->out != NULL) {
        for (size_t i = 0; i < dag->vertex_count; ++i) {
            free(dag->out[i].items);
        }
    }

    free(dag->out);
    free(dag->indegree);
    free(dag);
}

size_t int_dag_vertex_count(const IntDAG *dag) {
    return dag == NULL ? 0 : dag->vertex_count;
}

size_t int_dag_edge_count(const IntDAG *dag) {
    return dag == NULL ? 0 : dag->edge_count;
}

bool int_dag_has_edge(
    const IntDAG *dag,
    size_t from,
    size_t to
) {
    if (dag == NULL ||
        from >= dag->vertex_count ||
        to >= dag->vertex_count) {
        return false;
    }

    const SizeVec *vec = &dag->out[from];
    const size_t pos = lower_bound(vec, to);

    return pos < vec->count && vec->items[pos] == to;
}

static bool path_exists(
    const IntDAG *dag,
    size_t source,
    size_t target,
    bool *out_exists
) {
    if (dag == NULL || out_exists == NULL ||
        source >= dag->vertex_count ||
        target >= dag->vertex_count) {
        return false;
    }

    if (source == target) {
        *out_exists = true;
        return true;
    }

    if (dag->vertex_count > SIZE_MAX / sizeof(size_t)) {
        return false;
    }

    bool *visited = calloc(
        dag->vertex_count,
        sizeof *visited
    );
    size_t *stack = malloc(
        dag->vertex_count * sizeof *stack
    );

    if (visited == NULL || stack == NULL) {
        free(visited);
        free(stack);
        return false;
    }

    size_t top = 0;
    stack[top++] = source;
    visited[source] = true;

    bool found = false;

    while (top > 0 && !found) {
        const size_t u = stack[--top];
        const SizeVec *vec = &dag->out[u];

        for (size_t i = 0; i < vec->count; ++i) {
            const size_t v = vec->items[i];

            if (v == target) {
                found = true;
                break;
            }

            if (!visited[v]) {
                visited[v] = true;

                if (top >= dag->vertex_count) {
                    free(visited);
                    free(stack);
                    return false;
                }

                stack[top++] = v;
            }
        }
    }

    free(visited);
    free(stack);

    *out_exists = found;
    return true;
}

bool int_dag_add_edge(
    IntDAG *dag,
    size_t from,
    size_t to
) {
    if (dag == NULL ||
        from >= dag->vertex_count ||
        to >= dag->vertex_count ||
        from == to ||
        dag->edge_count == SIZE_MAX ||
        dag->indegree[to] == SIZE_MAX) {
        return false;
    }

    SizeVec *vec = &dag->out[from];
    const size_t pos = lower_bound(vec, to);

    if (pos < vec->count && vec->items[pos] == to) {
        return false;
    }

    bool creates_cycle = false;

    if (!path_exists(dag, to, from, &creates_cycle)) {
        return false;
    }

    if (creates_cycle) return false;

    if (!reserve_vec(vec, vec->count + 1)) return false;

    for (size_t i = vec->count; i > pos; --i) {
        vec->items[i] = vec->items[i - 1];
    }

    vec->items[pos] = to;
    ++vec->count;
    ++dag->indegree[to];
    ++dag->edge_count;

    return true;
}

bool int_dag_remove_edge(
    IntDAG *dag,
    size_t from,
    size_t to
) {
    if (dag == NULL ||
        from >= dag->vertex_count ||
        to >= dag->vertex_count) {
        return false;
    }

    SizeVec *vec = &dag->out[from];
    const size_t pos = lower_bound(vec, to);

    if (pos >= vec->count || vec->items[pos] != to) {
        return false;
    }

    if (dag->indegree[to] == 0 ||
        dag->edge_count == 0) {
        return false;
    }

    for (size_t i = pos + 1; i < vec->count; ++i) {
        vec->items[i - 1] = vec->items[i];
    }

    --vec->count;
    --dag->indegree[to];
    --dag->edge_count;

    return true;
}

bool int_dag_in_degree(
    const IntDAG *dag,
    size_t vertex,
    size_t *out_degree
) {
    if (dag == NULL || out_degree == NULL ||
        vertex >= dag->vertex_count) {
        return false;
    }

    *out_degree = dag->indegree[vertex];
    return true;
}

bool int_dag_out_degree(
    const IntDAG *dag,
    size_t vertex,
    size_t *out_degree
) {
    if (dag == NULL || out_degree == NULL ||
        vertex >= dag->vertex_count) {
        return false;
    }

    *out_degree = dag->out[vertex].count;
    return true;
}

bool int_dag_is_source(
    const IntDAG *dag,
    size_t vertex,
    bool *out_is_source
) {
    if (out_is_source == NULL) return false;

    size_t degree = 0;

    if (!int_dag_in_degree(dag, vertex, &degree)) {
        return false;
    }

    *out_is_source = degree == 0;
    return true;
}

bool int_dag_is_sink(
    const IntDAG *dag,
    size_t vertex,
    bool *out_is_sink
) {
    if (out_is_sink == NULL) return false;

    size_t degree = 0;

    if (!int_dag_out_degree(dag, vertex, &degree)) {
        return false;
    }

    *out_is_sink = degree == 0;
    return true;
}

bool int_dag_topological_sort(
    const IntDAG *dag,
    size_t *output,
    size_t capacity,
    size_t *out_written
) {
    if (dag == NULL || out_written == NULL) return false;

    *out_written = 0;

    if (dag->vertex_count == 0) return true;

    if (output == NULL || capacity < dag->vertex_count) {
        return false;
    }

    if (dag->vertex_count > SIZE_MAX / sizeof(size_t)) {
        return false;
    }

    size_t *remaining = malloc(
        dag->vertex_count * sizeof *remaining
    );
    size_t *queue = malloc(
        dag->vertex_count * sizeof *queue
    );

    if (remaining == NULL || queue == NULL) {
        free(remaining);
        free(queue);
        return false;
    }

    memcpy(
        remaining,
        dag->indegree,
        dag->vertex_count * sizeof *remaining
    );

    size_t head = 0;
    size_t tail = 0;

    for (size_t v = 0; v < dag->vertex_count; ++v) {
        if (remaining[v] == 0) {
            queue[tail++] = v;
        }
    }

    while (head < tail) {
        const size_t u = queue[head++];
        output[(*out_written)++] = u;

        const SizeVec *vec = &dag->out[u];

        for (size_t i = 0; i < vec->count; ++i) {
            const size_t v = vec->items[i];

            if (remaining[v] == 0) {
                free(remaining);
                free(queue);
                return false;
            }

            --remaining[v];

            if (remaining[v] == 0) {
                if (tail >= dag->vertex_count) {
                    free(remaining);
                    free(queue);
                    return false;
                }

                queue[tail++] = v;
            }
        }
    }

    free(remaining);
    free(queue);

    return *out_written == dag->vertex_count;
}

bool int_dag_validate(const IntDAG *dag) {
    if (dag == NULL) return false;

    if (dag->vertex_count == 0) {
        return dag->edge_count == 0;
    }

    if (dag->out == NULL || dag->indegree == NULL) {
        return false;
    }

    if (dag->vertex_count > SIZE_MAX / sizeof(size_t)) {
        return false;
    }

    size_t *actual_indegree = calloc(
        dag->vertex_count,
        sizeof *actual_indegree
    );
    size_t *order = malloc(
        dag->vertex_count * sizeof *order
    );

    if (actual_indegree == NULL || order == NULL) {
        free(actual_indegree);
        free(order);
        return false;
    }

    size_t edges = 0;

    for (size_t u = 0; u < dag->vertex_count; ++u) {
        const SizeVec *vec = &dag->out[u];

        if (vec->count > vec->capacity) {
            free(actual_indegree);
            free(order);
            return false;
        }

        if (vec->capacity > 0 && vec->items == NULL) {
            free(actual_indegree);
            free(order);
            return false;
        }

        for (size_t i = 0; i < vec->count; ++i) {
            const size_t v = vec->items[i];

            if (v >= dag->vertex_count || v == u) {
                free(actual_indegree);
                free(order);
                return false;
            }

            if (i > 0 && vec->items[i - 1] >= v) {
                free(actual_indegree);
                free(order);
                return false;
            }

            if (actual_indegree[v] == SIZE_MAX ||
                edges == SIZE_MAX) {
                free(actual_indegree);
                free(order);
                return false;
            }

            ++actual_indegree[v];
            ++edges;
        }
    }

    if (edges != dag->edge_count) {
        free(actual_indegree);
        free(order);
        return false;
    }

    for (size_t v = 0; v < dag->vertex_count; ++v) {
        if (actual_indegree[v] != dag->indegree[v]) {
            free(actual_indegree);
            free(order);
            return false;
        }
    }

    size_t written = 0;
    const bool acyclic = int_dag_topological_sort(
        dag,
        order,
        dag->vertex_count,
        &written
    );

    free(actual_indegree);
    free(order);

    return acyclic && written == dag->vertex_count;
}
