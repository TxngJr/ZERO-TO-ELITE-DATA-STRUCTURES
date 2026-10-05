#include "gap_buffer.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}int main(void){const size_t n=500000;GapBuffer*b=gap_buffer_create(1024);if(!b)return 1;const uint8_t x='x';struct timespec a,c;timespec_get(&a,TIME_UTC);for(size_t i=0;i<n;++i)if(!gap_buffer_insert(b,gap_buffer_gap_position(b),&x,1))return 1;timespec_get(&c,TIME_UTC);printf("size=%zu capacity=%zu seconds=%.9f\n",gap_buffer_size(b),gap_buffer_capacity(b),e(a,c));gap_buffer_free(b);return 0;}
