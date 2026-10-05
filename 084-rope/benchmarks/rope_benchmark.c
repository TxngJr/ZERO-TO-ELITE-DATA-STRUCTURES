#include "byte_rope.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}int main(void){const size_t n=100000;ByteRope*r=byte_rope_create(123);if(!r)return 1;const uint8_t x='x';struct timespec a,b;timespec_get(&a,TIME_UTC);for(size_t i=0;i<n;++i)if(!byte_rope_insert(r,byte_rope_size(r),&x,1))return 1;timespec_get(&b,TIME_UTC);volatile uint64_t sum=0;for(size_t i=0;i<n;i+=97){uint8_t v=0;if(!byte_rope_get(r,i,&v))return 1;sum+=v;}printf("size=%zu build=%.9f checksum=%llu\n",byte_rope_size(r),e(a,b),(unsigned long long)sum);byte_rope_free(r);return 0;}
