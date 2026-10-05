#ifndef CONSISTENT_HASH_RING_H
#define CONSISTENT_HASH_RING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ConsistentHashRing ConsistentHashRing;

ConsistentHashRing *consistent_hash_ring_create(size_t virtual_nodes_per_node);
void consistent_hash_ring_free(ConsistentHashRing *ring);

size_t consistent_hash_ring_node_count(const ConsistentHashRing *ring);
size_t consistent_hash_ring_point_count(const ConsistentHashRing *ring);

bool consistent_hash_ring_add_node(ConsistentHashRing *ring,uint64_t node_id);
bool consistent_hash_ring_remove_node(ConsistentHashRing *ring,uint64_t node_id);

bool consistent_hash_ring_lookup(
    const ConsistentHashRing *ring,
    const uint8_t *key,
    size_t key_length,
    uint64_t *out_node_id
);

bool consistent_hash_ring_validate(const ConsistentHashRing *ring);

#endif
