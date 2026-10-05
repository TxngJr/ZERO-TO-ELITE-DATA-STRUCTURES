#include "consistent_hash_ring.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double elapsed(struct timespec a,struct timespec b){return (double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
static void encode(uint64_t x,uint8_t out[8]){for(unsigned i=0;i<8;++i)out[i]=(uint8_t)(x>>(8U*i));}
int main(void){ConsistentHashRing *r=consistent_hash_ring_create(128);if(!r)return 1;for(uint64_t n=1;n<=100;++n)if(!consistent_hash_ring_add_node(r,n))return 1;const size_t q=1000000;uint8_t key[8];volatile uint64_t sum=0;struct timespec a,b;timespec_get(&a,TIME_UTC);for(uint64_t i=0;i<q;++i){uint64_t node=0;encode(i,key);if(!consistent_hash_ring_lookup(r,key,8,&node))return 1;sum+=node;}timespec_get(&b,TIME_UTC);printf("nodes=100 points=%zu queries=%zu seconds=%.9f checksum=%llu\n",consistent_hash_ring_point_count(r),q,elapsed(a,b),(unsigned long long)sum);consistent_hash_ring_free(r);return 0;}
