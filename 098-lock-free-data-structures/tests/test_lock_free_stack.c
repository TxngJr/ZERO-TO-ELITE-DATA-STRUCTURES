#include "lock_free_stack.h"
#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
enum{THREADS=8,PER_THREAD=5000,TOTAL=THREADS*PER_THREAD,MIX_PRODUCERS=4,MIX_CONSUMERS=4,MIX_PER_PRODUCER=2000,MIX_TOTAL=MIX_PRODUCERS*MIX_PER_PRODUCER};
typedef struct{LockFreeStack*s;int id;}PA;typedef struct{LockFreeStack*s;_Atomic unsigned char*seen;_Atomic size_t*count;}PO;
static void*push_worker(void*p){PA*a=p;int base=a->id*PER_THREAD;for(int i=0;i<PER_THREAD;++i){bool x=false;assert(lfs_push(a->s,base+i,&x)&&x);}return NULL;}
static void*pop_worker(void*p){PO*a=p;for(;;){int v=-1;bool x=false;assert(lfs_pop(a->s,&v,&x));if(!x)break;assert(v>=0&&v<TOTAL);assert(atomic_exchange(&a->seen[v],1U)==0U);atomic_fetch_add(a->count,1U);}return NULL;}
typedef struct{LockFreeStack*s;int id;_Atomic int*done;}MP;typedef struct{LockFreeStack*s;_Atomic int*done;_Atomic unsigned char*seen;_Atomic size_t*count;}MC;
static void*mix_push(void*p){MP*a=p;int base=a->id*MIX_PER_PRODUCER;for(int i=0;i<MIX_PER_PRODUCER;++i){bool x=false;assert(lfs_push(a->s,base+i,&x)&&x);}atomic_fetch_add_explicit(a->done,1,memory_order_release);return NULL;}
static void*mix_pop(void*p){MC*a=p;for(;;){int v=-1;bool x=false;assert(lfs_pop(a->s,&v,&x));if(x){assert(v>=0&&v<MIX_TOTAL);assert(atomic_exchange(&a->seen[v],1U)==0U);atomic_fetch_add(a->count,1U);continue;}if(atomic_load_explicit(a->done,memory_order_acquire)==MIX_PRODUCERS)break;}return NULL;}
int main(void){
 LockFreeStack*s=lfs_create(TOTAL);assert(s&&lfs_platform_lock_free(s));pthread_t th[THREADS];PA pa[THREADS];for(int i=0;i<THREADS;++i){pa[i]=(PA){s,i};assert(pthread_create(&th[i],NULL,push_worker,&pa[i])==0);}for(int i=0;i<THREADS;++i)pthread_join(th[i],NULL);assert(lfs_size_approx(s)==TOTAL&&lfs_validate_quiescent(s));
 _Atomic unsigned char*seen=calloc(TOTAL,sizeof*seen);assert(seen);_Atomic size_t cnt;atomic_init(&cnt,0U);PO po={s,seen,&cnt};for(int i=0;i<THREADS;++i)assert(pthread_create(&th[i],NULL,pop_worker,&po)==0);for(int i=0;i<THREADS;++i)pthread_join(th[i],NULL);assert(atomic_load(&cnt)==TOTAL);for(int i=0;i<TOTAL;++i)assert(atomic_load(&seen[i])==1U);assert(lfs_size_approx(s)==0&&lfs_validate_quiescent(s));bool x=true;assert(lfs_push(s,999,&x)&&!x);
 LockFreeStack*m=lfs_create(MIX_TOTAL);assert(m&&lfs_platform_lock_free(m));_Atomic int done;_Atomic size_t mc;atomic_init(&done,0);atomic_init(&mc,0U);_Atomic unsigned char*ms=calloc(MIX_TOTAL,sizeof*ms);assert(ms);pthread_t pp[MIX_PRODUCERS],cc[MIX_CONSUMERS];MP mpa[MIX_PRODUCERS];MC mca={m,&done,ms,&mc};for(int i=0;i<MIX_CONSUMERS;++i)assert(pthread_create(&cc[i],NULL,mix_pop,&mca)==0);for(int i=0;i<MIX_PRODUCERS;++i){mpa[i]=(MP){m,i,&done};assert(pthread_create(&pp[i],NULL,mix_push,&mpa[i])==0);}for(int i=0;i<MIX_PRODUCERS;++i)pthread_join(pp[i],NULL);for(int i=0;i<MIX_CONSUMERS;++i)pthread_join(cc[i],NULL);assert(atomic_load(&mc)==MIX_TOTAL&&lfs_size_approx(m)==0&&lfs_validate_quiescent(m));free(ms);lfs_free(m);
 printf("Lock-free stack tests passed; push_cas_failures=%zu pop_cas_failures=%zu\n",lfs_push_cas_failures(s),lfs_pop_cas_failures(s));free(seen);lfs_free(s);return 0;
}
