#include "u64_hyperloglog.h"
#include <assert.h>
#include <stdio.h>
int main(void){U64HyperLogLog *h=u64_hll_create(12);assert(h);for(uint64_t i=0;i<50000;++i)assert(u64_hll_add(h,i));double e=0;assert(u64_hll_estimate(h,&e));printf("registers=%zu estimate=%.1f\n",u64_hll_register_count(h),e);u64_hll_free(h);return 0;}
