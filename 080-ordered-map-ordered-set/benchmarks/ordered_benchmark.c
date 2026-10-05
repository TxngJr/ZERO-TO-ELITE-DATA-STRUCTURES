#include "u64_ordered.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return (double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}int main(void){const size_t n=50000;U64OrderedMap*m=u64_ordered_map_create();if(!m)return 1;struct timespec a,b;timespec_get(&a,TIME_UTC);for(uint64_t i=n;i-->0;)if(!u64_ordered_map_put(m,i,(int64_t)i))return 1;timespec_get(&b,TIME_UTC);printf("descending inserts=%zu seconds=%.9f\n",n,e(a,b));u64_ordered_map_free(m);return 0;}
