#include "concurrent_int_set.h"
#include <pthread.h>
#include <stdio.h>
#include <time.h>
enum{THREADS=8,OPS=10000};
typedef struct{ConcurrentIntSet*s;int id;}A;
static void*w(void*p){A*a=p;int base=a->id*OPS;for(int i=0;i<OPS;++i){bool x;if(!cis_insert(a->s,base+i,&x))return(void*)1;}return NULL;}
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){ConcurrentIntSet*s=cis_create();if(!s)return 1;pthread_t t[THREADS];A a[THREADS];struct timespec x,y;timespec_get(&x,TIME_UTC);for(int i=0;i<THREADS;++i){a[i]=(A){s,i};if(pthread_create(&t[i],NULL,w,&a[i]))return 2;}for(int i=0;i<THREADS;++i){void*r=NULL;pthread_join(t[i],&r);if(r)return 3;}timespec_get(&y,TIME_UTC);size_t n=0;cis_size(s,&n);printf("threads=%d ops_per_thread=%d final=%zu seconds=%.9f ops_per_sec=%.0f\n",THREADS,OPS,n,e(x,y),(double)(THREADS*OPS)/e(x,y));cis_free(s);return 0;}
