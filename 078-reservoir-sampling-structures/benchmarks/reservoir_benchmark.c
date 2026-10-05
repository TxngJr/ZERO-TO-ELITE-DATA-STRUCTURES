#include "u64_reservoir.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double elapsed(struct timespec a,struct timespec b){return (double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){const uint64_t n=10000000;U64Reservoir *r=u64_reservoir_create(1024,123456789);if(!r)return 1;struct timespec a,b;timespec_get(&a,TIME_UTC);for(uint64_t i=0;i<n;++i)if(!u64_reservoir_add(r,i))return 1;timespec_get(&b,TIME_UTC);printf("seen=%llu k=%zu seconds=%.9f\n",(unsigned long long)u64_reservoir_seen(r),u64_reservoir_capacity(r),elapsed(a,b));u64_reservoir_free(r);return 0;}
