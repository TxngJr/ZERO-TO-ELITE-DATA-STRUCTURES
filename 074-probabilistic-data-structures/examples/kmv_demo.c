#include "u64_kmv_sketch.h"
#include <assert.h>
#include <stdio.h>
int main(void){U64KmvSketch *s=u64_kmv_sketch_create(256);assert(s);for(uint64_t i=0;i<10000;++i)assert(u64_kmv_sketch_add(s,i));double e=0;assert(u64_kmv_sketch_estimate(s,&e));printf("retained=%zu estimate=%.1f\n",u64_kmv_sketch_retained(s),e);u64_kmv_sketch_free(s);return 0;}
