#include "u64_count_min_sketch.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double elapsed(struct timespec a,struct timespec b){return (double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){const size_t n=1000000;U64CountMinSketch *s=u64_cms_create(4096,5);if(!s)return 1;struct timespec a,b;timespec_get(&a,TIME_UTC);for(uint64_t i=0;i<n;++i)if(!u64_cms_add(s,i%10000,1))return 1;timespec_get(&b,TIME_UTC);uint64_t e=0;if(!u64_cms_estimate(s,42,&e))return 1;printf("updates=%zu estimate42=%llu seconds=%.9f\n",n,(unsigned long long)e,elapsed(a,b));u64_cms_free(s);return 0;}
