#include "concurrent_hash_map.h"
#include <pthread.h>
#include <stdio.h>
#include <time.h>
enum{T=8,N=50000};typedef struct{ConcurrentHashMap*m;int id;}A;
static void*w(void*p){A*a=p;int b=a->id*N;for(int i=0;i<N;++i){bool ins=false;if(!chm_put(a->m,b+i,b+i,&ins)||!ins)return(void*)1;}return NULL;}
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){ConcurrentHashMap*m=chm_create(8);if(!m)return 1;pthread_t t[T];A a[T];struct timespec x,y;timespec_get(&x,TIME_UTC);for(int i=0;i<T;++i){a[i]=(A){m,i};if(pthread_create(&t[i],NULL,w,&a[i]))return 2;}for(int i=0;i<T;++i){void*r=NULL;pthread_join(t[i],&r);if(r)return 3;}timespec_get(&y,TIME_UTC);size_t buckets=0;chm_bucket_count(m,&buckets);printf("puts=%d seconds=%.6f size=%zu buckets=%zu resizes=%zu\n",T*N,e(x,y),chm_size(m),buckets,chm_resize_count(m));chm_free(m);return 0;}
