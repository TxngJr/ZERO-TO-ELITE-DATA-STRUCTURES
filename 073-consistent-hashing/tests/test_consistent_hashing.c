#include "consistent_hash_ring.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static void encode(uint64_t x,uint8_t out[8]){for(unsigned i=0;i<8;++i)out[i]=(uint8_t)(x>>(8U*i));}

int main(void){
    ConsistentHashRing *r=consistent_hash_ring_create(128);assert(r);
    assert(consistent_hash_ring_add_node(r,10));assert(consistent_hash_ring_add_node(r,20));assert(consistent_hash_ring_add_node(r,30));
    assert(consistent_hash_ring_validate(r));
    enum { KEYS=50000 };
    uint64_t before[KEYS];uint8_t key[8];
    size_t counts_before[3]={0};
    for(uint64_t i=0;i<KEYS;++i){encode(i,key);assert(consistent_hash_ring_lookup(r,key,8,&before[i]));if(before[i]==10)++counts_before[0];else if(before[i]==20)++counts_before[1];else if(before[i]==30)++counts_before[2];else assert(false);}
    for(size_t i=0;i<3;++i)assert(counts_before[i]>KEYS/6);

    assert(consistent_hash_ring_add_node(r,40));
    size_t moved=0,moved_to_new=0;
    for(uint64_t i=0;i<KEYS;++i){uint64_t now=0;encode(i,key);assert(consistent_hash_ring_lookup(r,key,8,&now));if(now!=before[i]){++moved;if(now==40)++moved_to_new;}}
    assert(moved==moved_to_new);
    assert(moved>KEYS/10&&moved<KEYS/2);

    assert(consistent_hash_ring_remove_node(r,40));
    for(uint64_t i=0;i<KEYS;++i){uint64_t now=0;encode(i,key);assert(consistent_hash_ring_lookup(r,key,8,&now));assert(now==before[i]);}
    assert(consistent_hash_ring_validate(r));
    consistent_hash_ring_free(r);
    puts("Consistent Hashing tests passed");return 0;
}
