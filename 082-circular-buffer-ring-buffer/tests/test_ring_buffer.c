#include "u64_ring_buffer.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
static uint32_t next_rng(uint32_t*s){*s=*s*1664525u+1013904223u;return*s;}
int main(void){enum{CAP=64,OPS=50000};U64RingBuffer*b=u64_ring_buffer_create(CAP);assert(b);uint64_t model[CAP]={0};size_t n=0;uint32_t rng=0x82A12345u;
for(int op=0;op<OPS;++op){unsigned kind=next_rng(&rng)%4U;if(kind<2){uint64_t v=((uint64_t)next_rng(&rng)<<32)|next_rng(&rng);bool ok=u64_ring_buffer_push(b,v);assert(ok==(n<CAP));if(ok)model[n++]=v;}else if(kind==2){uint64_t got=0;bool ok=u64_ring_buffer_pop(b,&got);assert(ok==(n>0));if(ok){assert(got==model[0]);memmove(&model[0],&model[1],(n-1)*sizeof model[0]);--n;}}else{if(n){size_t i=next_rng(&rng)%n;uint64_t got=0;assert(u64_ring_buffer_get(b,i,&got));assert(got==model[i]);}}assert(u64_ring_buffer_size(b)==n);assert(u64_ring_buffer_validate(b));}
for(size_t i=0;i<n;++i){uint64_t got=0;assert(u64_ring_buffer_get(b,i,&got)&&got==model[i]);}
u64_ring_buffer_clear(b);assert(u64_ring_buffer_is_empty(b));assert(u64_ring_buffer_validate(b));u64_ring_buffer_free(b);puts("Ring Buffer tests passed");return 0;}
