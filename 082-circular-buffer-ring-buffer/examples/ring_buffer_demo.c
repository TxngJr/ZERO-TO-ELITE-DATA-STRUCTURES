#include "u64_ring_buffer.h"
#include <assert.h>
#include <stdio.h>
int main(void){U64RingBuffer*b=u64_ring_buffer_create(4);assert(b);u64_ring_buffer_push(b,10);u64_ring_buffer_push(b,20);uint64_t x=0;u64_ring_buffer_pop(b,&x);u64_ring_buffer_push(b,30);u64_ring_buffer_push(b,40);u64_ring_buffer_push(b,50);for(size_t i=0;i<u64_ring_buffer_size(b);++i){u64_ring_buffer_get(b,i,&x);printf("%llu ",(unsigned long long)x);}putchar('\n');u64_ring_buffer_free(b);return 0;}
