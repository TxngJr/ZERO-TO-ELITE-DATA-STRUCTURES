#include "locality_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
enum{N=4000000};static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
static unsigned long long run(const unsigned long long*a,const size_t*order){volatile unsigned long long sum=0;for(size_t i=0;i<N;++i)sum+=a[order[i]];return sum;}
int main(void){unsigned long long*a=malloc((size_t)N*sizeof(*a));size_t*seq=malloc((size_t)N*sizeof(*seq));size_t*perm=malloc((size_t)N*sizeof(*perm));if(!a||!seq||!perm)return 1;for(size_t i=0;i<N;++i)a[i]=(unsigned long long)i*17U;if(!loc_generate_sequential(N,seq)||!loc_generate_strided_permutation(N,3999999U,perm))return 2;struct timespec x,y,z;timespec_get(&x,TIME_UTC);unsigned long long s1=run(a,seq);timespec_get(&y,TIME_UTC);unsigned long long s2=run(a,perm);timespec_get(&z,TIME_UTC);printf("sequential_seconds=%.6f permuted_seconds=%.6f checksums=%llu,%llu\n",e(x,y),e(y,z),s1,s2);free(perm);free(seq);free(a);return 0;}
