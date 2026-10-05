#include "u64_linked_hash_map.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return (double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}int main(void){const size_t n=200000;U64LinkedHashMap*m=u64_lhm_create(1024);if(!m)return 1;struct timespec a,b;timespec_get(&a,TIME_UTC);for(uint64_t i=0;i<n;++i)if(!u64_lhm_put(m,i,(int64_t)i))return 1;timespec_get(&b,TIME_UTC);printf("n=%zu seconds=%.9f\n",n,e(a,b));u64_lhm_free(m);return 0;}
