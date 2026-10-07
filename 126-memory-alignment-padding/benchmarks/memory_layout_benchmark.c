#include "memory_layout.h"
#include <stdio.h>
#include <time.h>
enum{N=2000000,ROUNDS=4};static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
static double run(MaAlignedArray*a){struct timespec x,y;timespec_get(&x,TIME_UTC);volatile unsigned long long sum=0;for(int r=0;r<ROUNDS;++r)for(size_t i=0;i<N;++i){unsigned long long*p=ma_aligned_array_get(a,i);p[0]=(unsigned long long)(i+r);sum+=p[0];}timespec_get(&y,TIME_UTC);printf("stride=%zu bytes=%zu checksum=%llu seconds=%.6f\n",ma_aligned_array_stride(a),ma_aligned_array_storage_bytes(a),sum,e(x,y));return e(x,y);}
int main(void){MaAlignedArray*natural=ma_aligned_array_create(N,24,8);MaAlignedArray*wide=ma_aligned_array_create(N,24,64);if(!natural||!wide)return 1;run(natural);run(wide);ma_aligned_array_free(wide);ma_aligned_array_free(natural);return 0;}
