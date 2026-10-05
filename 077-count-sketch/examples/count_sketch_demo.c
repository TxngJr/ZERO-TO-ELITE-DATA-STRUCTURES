#include "u64_count_sketch.h"
#include <assert.h>
#include <stdio.h>
int main(void){U64CountSketch *s=u64_count_sketch_create(1024,5);assert(s);for(int i=0;i<1000;++i)assert(u64_count_sketch_update(s,(uint64_t)(i%10),1));for(int i=0;i<20;++i)assert(u64_count_sketch_update(s,3,-1));int64_t e=0;assert(u64_count_sketch_estimate(s,3,&e));printf("estimate(3)=%lld\n",(long long)e);u64_count_sketch_free(s);return 0;}
