#ifndef NETWORK_STRUCTURES_H
#define NETWORK_STRUCTURES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct RouteTrie RouteTrie;
typedef struct FlowTable FlowTable;
typedef struct { uint32_t src_ip,dst_ip; uint16_t src_port,dst_port; uint8_t protocol; } FlowKey;
RouteTrie *rt_create(void);
void rt_free(RouteTrie *trie);
bool rt_add(RouteTrie *trie,uint32_t prefix,uint8_t prefix_len,uint32_t next_hop);
bool rt_remove(RouteTrie *trie,uint32_t prefix,uint8_t prefix_len,bool *out_removed);
bool rt_lookup(const RouteTrie *trie,uint32_t address,uint32_t *out_next_hop,uint8_t *out_prefix_len,bool *out_found);
size_t rt_node_count(const RouteTrie *trie);
bool rt_validate(const RouteTrie *trie);
FlowTable *flow_create(size_t initial_capacity);
void flow_free(FlowTable *table);
bool flow_put(FlowTable *table,FlowKey key,uint64_t value,bool *out_inserted);
bool flow_get(const FlowTable *table,FlowKey key,uint64_t *out_value,bool *out_found);
bool flow_remove(FlowTable *table,FlowKey key,bool *out_removed);
size_t flow_size(const FlowTable *table);
size_t flow_capacity(const FlowTable *table);
bool flow_validate(const FlowTable *table);
#endif
