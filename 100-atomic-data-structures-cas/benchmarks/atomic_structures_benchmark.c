#include "atomic_structures.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <time.h>
enum{T=8,N=100000};typedef struct{AtomicTaggedValue*v;_Atomic size_t*r;}A;
static void*w(void*p){A*a=p;for(int i=0;i<N;++i){size_t r=0;if(!atv_fetch_increment(a->v,NULL,&r))return(void*)1;atomic_fetch_add(a->r,r);}return NULL;}
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){AtomicTaggedValue*v=atv_create(0);if(!v||!atv_platform_lock_free(v))return 1;_Atomic size_t retries;atomic_init(&retries,0);A a={v,&retries};pthread_t t[T];struct timespec x,y;timespec_get(&x,TIME_UTC);for(int i=0;i<T;++i)if(pthread_create(&t[i],NULL,w,&a))return 2;for(int i=0;i<T;++i){void*r=NULL;pthread_join(t[i],&r);if(r)return 3;}timespec_get(&y,TIME_UTC);ATVSnapshot s;atv_load(v,&s);printf("increments=%d seconds=%.6f cas_retries=%zu final=%u version=%u\n",T*N,e(x,y),atomic_load(&retries),s.value,s.version);atv_free(v);return 0;}
