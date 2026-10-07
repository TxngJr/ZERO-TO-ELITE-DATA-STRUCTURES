#ifndef GRAPH_DB_H
#define GRAPH_DB_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct GraphDb GraphDb;
GraphDb *gdb_create(size_t node_capacity,size_t edge_capacity);
void gdb_free(GraphDb *graph);
bool gdb_add_node(GraphDb *graph,uint64_t id,uint32_t label,int64_t property);
bool gdb_add_edge(GraphDb *graph,uint64_t src_id,uint64_t dst_id,uint32_t type,int64_t weight);
bool gdb_build_indexes(GraphDb *graph);
bool gdb_node(GraphDb *graph,uint64_t id,uint32_t *out_label,int64_t *out_property);
bool gdb_out_neighbors(const GraphDb *graph,uint64_t id,uint32_t type,uint64_t *out_ids,size_t out_capacity,size_t *out_count);
bool gdb_in_neighbors(const GraphDb *graph,uint64_t id,uint32_t type,uint64_t *out_ids,size_t out_capacity,size_t *out_count);
bool gdb_nodes_with_label(const GraphDb *graph,uint32_t label,uint64_t *out_ids,size_t out_capacity,size_t *out_count);
bool gdb_shortest_hops(const GraphDb *graph,uint64_t source_id,uint64_t target_id,uint32_t edge_type,size_t *out_hops,bool *out_found);
size_t gdb_node_count(const GraphDb *graph);
size_t gdb_edge_count(const GraphDb *graph);
bool gdb_indexes_fresh(const GraphDb *graph);
bool gdb_validate(const GraphDb *graph);
#endif
