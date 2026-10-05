#include "u64_kmv_sketch.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double elapsed(struct timespec a,struct timespec b){return (double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){const size_t n=1000000;U64KmvSketch *s=u64_kmv_sketch_create(2048);if(!s)return 1;struct timespec a,b;timespec_get(&a,TIME_UTC);for(uint64_t i=0;i<n;++i)if(!u64_kmv_sketch_add(s,i))return 1;timespec_get(&b,TIME_UTC);double e=0;if(!u64_kmv_sketch_estimate(s,&e))return 1;printf("n=%zu k=%zu estimate=%.1f seconds=%.9f\n",n,u64_kmv_sketch_k(s),e,elapsed(a,b));u64_kmv_sketch_free(s);return 0;}
