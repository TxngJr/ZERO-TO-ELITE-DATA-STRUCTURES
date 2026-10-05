#include "u64_ring_buffer.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}int main(void){const size_t ops=10000000;U64RingBuffer*b=u64_ring_buffer_create(1024);if(!b)return 1;struct timespec a,c;timespec_get(&a,TIME_UTC);uint64_t x=0;for(uint64_t i=0;i<ops;++i){if(!u64_ring_buffer_push(b,i))return 1;if(u64_ring_buffer_size(b)==1024)for(int j=0;j<512;++j)if(!u64_ring_buffer_pop(b,&x))return 1;}timespec_get(&c,TIME_UTC);printf("ops=%zu remaining=%zu seconds=%.9f checksum=%llu\n",ops,u64_ring_buffer_size(b),e(a,c),(unsigned long long)x);u64_ring_buffer_free(b);return 0;}
