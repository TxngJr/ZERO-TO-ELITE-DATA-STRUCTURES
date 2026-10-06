#include "thread_safe_structures.h"
#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
enum{PRODUCERS=4,CONSUMERS=4,ITEMS_PER_PRODUCER=5000,MAP_THREADS=8,MAP_PER_THREAD=3000,TOTAL_ITEMS=PRODUCERS*ITEMS_PER_PRODUCER};
typedef struct{ThreadSafeQueue*q;int id;}QA;
typedef struct{ThreadSafeQueue*q;_Atomic unsigned char*seen;_Atomic size_t*count;}CA;
static void*producer(void*p){QA*a=p;int base=a->id*ITEMS_PER_PRODUCER;for(int i=0;i<ITEMS_PER_PRODUCER;++i){bool x=false;assert(tsq_push_wait(a->q,base+i,&x)&&x);}return NULL;}
static void*consumer(void*p){CA*a=p;for(;;){int v=-1;bool x=false;assert(tsq_pop_wait(a->q,&v,&x));if(!x)break;assert(v>=0&&v<TOTAL_ITEMS);assert(atomic_exchange_explicit(&a->seen[v],1U,memory_order_relaxed)==0U);atomic_fetch_add_explicit(a->count,1U,memory_order_relaxed);}return NULL;}
typedef struct{ThreadSafeIntMap*m;int id;}MA;
static void*map_put(void*p){MA*a=p;int base=a->id*MAP_PER_THREAD;for(int i=0;i<MAP_PER_THREAD;++i){bool x=false;assert(tsm_put(a->m,base+i,(base+i)*3,&x)&&x);}return NULL;}
static void*map_remove_even(void*p){MA*a=p;int base=a->id*MAP_PER_THREAD;for(int i=0;i<MAP_PER_THREAD;i+=2){bool x=false;assert(tsm_remove(a->m,base+i,&x)&&x);}return NULL;}
int main(void){
    ThreadSafeQueue*q=tsq_create(127);assert(q&&tsq_validate_quiescent(q));_Atomic unsigned char*seen=calloc(TOTAL_ITEMS,sizeof*seen);assert(seen);_Atomic size_t count;atomic_init(&count,0U);
    pthread_t p[PRODUCERS],c[CONSUMERS];QA pa[PRODUCERS];CA ca={q,seen,&count};for(int i=0;i<CONSUMERS;++i)assert(pthread_create(&c[i],NULL,consumer,&ca)==0);for(int i=0;i<PRODUCERS;++i){pa[i]=(QA){q,i};assert(pthread_create(&p[i],NULL,producer,&pa[i])==0);}for(int i=0;i<PRODUCERS;++i)pthread_join(p[i],NULL);assert(tsq_close(q));for(int i=0;i<CONSUMERS;++i)pthread_join(c[i],NULL);assert(atomic_load(&count)==TOTAL_ITEMS);for(int i=0;i<TOTAL_ITEMS;++i)assert(atomic_load(&seen[i])==1U);size_t qs=1;assert(tsq_size(q,&qs)&&qs==0&&tsq_validate_quiescent(q));free(seen);tsq_free(q);
    ThreadSafeIntMap*m=tsm_create(32);assert(m);pthread_t mt[MAP_THREADS];MA ma[MAP_THREADS];for(int i=0;i<MAP_THREADS;++i){ma[i]=(MA){m,i};assert(pthread_create(&mt[i],NULL,map_put,&ma[i])==0);}for(int i=0;i<MAP_THREADS;++i)pthread_join(mt[i],NULL);size_t ms=0;assert(tsm_size(m,&ms)&&ms==(size_t)MAP_THREADS*MAP_PER_THREAD);assert(tsm_validate_quiescent(m));for(int i=0;i<MAP_THREADS;++i)assert(pthread_create(&mt[i],NULL,map_remove_even,&ma[i])==0);for(int i=0;i<MAP_THREADS;++i)pthread_join(mt[i],NULL);assert(tsm_size(m,&ms)&&ms==(size_t)MAP_THREADS*MAP_PER_THREAD/2U);for(int k=0;k<MAP_THREADS*MAP_PER_THREAD;++k){int v=0;bool f=false;assert(tsm_get(m,k,&v,&f));assert(f==(k%2!=0));if(f)assert(v==k*3);}assert(tsm_validate_quiescent(m));tsm_free(m);puts("Thread-safe queue/map tests passed");return 0;
}
