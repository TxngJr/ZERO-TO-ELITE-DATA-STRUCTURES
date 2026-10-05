#include "u64_multi.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}int main(void){const size_t n=100000;U64Multiset*s=u64_multiset_create();if(!s)return 1;struct timespec a,b;timespec_get(&a,TIME_UTC);for(uint64_t i=0;i<n;++i)if(!u64_multiset_add(s,i%1000,1))return 1;timespec_get(&b,TIME_UTC);printf("total=%zu distinct=%zu seconds=%.9f\n",u64_multiset_size(s),u64_multiset_distinct(s),e(a,b));u64_multiset_free(s);return 0;}
