#ifndef INT_DAG_H
#define INT_DAG_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntDAG IntDAG;

IntDAG *int_dag_create(size_t vertex_count);
void int_dag_free(IntDAG *dag);

size_t int_dag_vertex_count(const IntDAG *dag);
size_t int_dag_edge_count(const IntDAG *dag);

bool int_dag_add_edge(IntDAG *dag, size_t from, size_t to);
bool int_dag_remove_edge(IntDAG *dag, size_t from, size_t to);
bool int_dag_has_edge(const IntDAG *dag, size_t from, size_t to);

bool int_dag_in_degree(
    const IntDAG *dag,
    size_t vertex,
    size_t *out_degree
);
bool int_dag_out_degree(
    const IntDAG *dag,
    size_t vertex,
    size_t *out_degree
);
bool int_dag_is_source(
    const IntDAG *dag,
    size_t vertex,
    bool *out_is_source
);
bool int_dag_is_sink(
    const IntDAG *dag,
    size_t vertex,
    bool *out_is_sink
);

bool int_dag_topological_sort(
    const IntDAG *dag,
    size_t *output,
    size_t capacity,
    size_t *out_written
);

bool int_dag_validate(const IntDAG *dag);

#endif
