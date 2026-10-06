#include "concurrent_mpmc_ring.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <time.h>
enum{P=4,C=4,N=250000,TOTAL=P*N};typedef struct{ConcurrentMpmcRing*r;int id;}PA;typedef struct{ConcurrentMpmcRing*r;_Atomic size_t*count;}CA;
static void*prod(void*x){PA*a=x;int b=a->id*N;for(int i=0;i<N;++i)for(;;){bool ok=false;if(!cmr_try_push(a->r,b+i,&ok))return(void*)1;if(ok)break;}return NULL;}
static void*cons(void*x){CA*a=x;while(atomic_load(a->count)<TOTAL){int v;bool ok=false;if(!cmr_try_pop(a->r,&v,&ok))return(void*)1;if(ok)atomic_fetch_add(a->count,1);}return NULL;}
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){ConcurrentMpmcRing*r=cmr_create(1024);if(!r||!cmr_platform_lock_free(r))return 1;_Atomic size_t count;atomic_init(&count,0);pthread_t p[P],c[C];PA pa[P];CA ca={r,&count};struct timespec x,y;timespec_get(&x,TIME_UTC);for(int i=0;i<C;++i)if(pthread_create(&c[i],NULL,cons,&ca))return 2;for(int i=0;i<P;++i){pa[i]=(PA){r,i};if(pthread_create(&p[i],NULL,prod,&pa[i]))return 3;}for(int i=0;i<P;++i)pthread_join(p[i],NULL);for(int i=0;i<C;++i)pthread_join(c[i],NULL);timespec_get(&y,TIME_UTC);double s=e(x,y);printf("transfers=%d seconds=%.6f transfers_per_sec=%.0f\n",TOTAL,s,(double)TOTAL/s);cmr_free(r);return 0;}
