#include "u64_count_min_sketch.h"
#include <assert.h>
#include <stdio.h>
int main(void){U64CountMinSketch *s=u64_cms_create(1024,5);assert(s);for(int i=0;i<1000;++i)assert(u64_cms_add(s,(uint64_t)(i%10),1));uint64_t e=0;assert(u64_cms_estimate(s,3,&e));printf("estimate(3)=%llu total=%llu\n",(unsigned long long)e,(unsigned long long)u64_cms_total_weight(s));u64_cms_free(s);return 0;}
