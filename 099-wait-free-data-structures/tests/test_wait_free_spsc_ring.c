#include "wait_free_spsc_ring.h"
#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
enum{ITEMS=500000,CAPACITY=257};
typedef struct{WaitFreeSpscRing*r;_Atomic size_t*miss;}A;
static void*producer(void*p){A*a=p;for(int i=0;i<ITEMS;++i)for(;;){bool x=false;assert(wfr_try_push(a->r,i,&x));if(x)break;atomic_fetch_add(a->miss,1U);}return NULL;}
static void*consumer(void*p){A*a=p;for(int expected=0;expected<ITEMS;++expected)for(;;){int v=-1;bool x=false;assert(wfr_try_pop(a->r,&v,&x));if(!x){atomic_fetch_add(a->miss,1U);continue;}assert(v==expected);break;}return NULL;}
int main(void){WaitFreeSpscRing*r=wfr_create(CAPACITY);assert(r&&wfr_platform_lock_free(r)&&wfr_usable_capacity(r)==CAPACITY-1U&&wfr_validate_quiescent(r));_Atomic size_t full,empty;atomic_init(&full,0U);atomic_init(&empty,0U);A pa={r,&full},ca={r,&empty};pthread_t p,c;assert(pthread_create(&c,NULL,consumer,&ca)==0);assert(pthread_create(&p,NULL,producer,&pa)==0);pthread_join(p,NULL);pthread_join(c,NULL);assert(wfr_validate_quiescent(r));int v=0;bool x=true;assert(wfr_try_pop(r,&v,&x)&&!x);printf("Wait-free SPSC ring tests passed; caller_full_retries=%zu caller_empty_retries=%zu\n",atomic_load(&full),atomic_load(&empty));wfr_free(r);return 0;}
