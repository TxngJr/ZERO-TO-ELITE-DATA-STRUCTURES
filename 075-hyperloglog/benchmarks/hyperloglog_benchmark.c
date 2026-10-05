#include "u64_hyperloglog.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double elapsed(struct timespec a,struct timespec b){return (double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){const uint64_t n=1000000;U64HyperLogLog *h=u64_hll_create(14);if(!h)return 1;struct timespec a,b;timespec_get(&a,TIME_UTC);for(uint64_t i=0;i<n;++i)if(!u64_hll_add(h,i))return 1;timespec_get(&b,TIME_UTC);double e=0;if(!u64_hll_estimate(h,&e))return 1;printf("n=%llu p=%u m=%zu estimate=%.1f error_pct=%.4f seconds=%.9f\n",(unsigned long long)n,u64_hll_precision(h),u64_hll_register_count(h),e,100.0*(e-(double)n)/(double)n,elapsed(a,b));u64_hll_free(h);return 0;}
