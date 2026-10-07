#include "vector_index.h"
#include <stdio.h>
#include <time.h>
enum{N=50000,D=32,Q=200,K=10};static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){VectorIndex*i=vi_create(D,N);if(!i)return 1;float v[D];for(size_t n=0;n<N;++n){for(size_t d=0;d<D;++d)v[d]=(float)((n*31U+d*7U)%4093U)/4093.0f+.001f;size_t id;if(!vi_add(i,v,&id))return 2;}VectorSearchResult r[K];struct timespec a,b;timespec_get(&a,TIME_UTC);size_t checksum=0;for(size_t q=0;q<Q;++q){size_t n=0;if(!vi_search(i,vi_vector(i,(q*251U)%N),K,VECTOR_METRIC_L2,r,&n))return 3;checksum+=r[0].id;}timespec_get(&b,TIME_UTC);printf("vectors=%d dims=%d queries=%d seconds=%.6f checksum=%zu\n",N,D,Q,e(a,b),checksum);vi_free(i);return 0;}
