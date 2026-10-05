#include "consistent_hash_ring.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){ConsistentHashRing *r=consistent_hash_ring_create(64);assert(r);assert(consistent_hash_ring_add_node(r,101));assert(consistent_hash_ring_add_node(r,202));uint64_t node=0;const uint8_t *k=(const uint8_t*)"customer:42";assert(consistent_hash_ring_lookup(r,k,strlen((const char*)k),&node));printf("node=%llu points=%zu\n",(unsigned long long)node,consistent_hash_ring_point_count(r));consistent_hash_ring_free(r);return 0;}
