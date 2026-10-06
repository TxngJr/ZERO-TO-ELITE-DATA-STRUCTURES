#include "wait_free_spsc_ring.h"
#include <pthread.h>
#include <stdio.h>
#include <time.h>
enum{N=2000000};typedef struct{WaitFreeSpscRing*r;}A;
static void*p(void*x){A*a=x;for(int i=0;i<N;++i){bool ok=false;do{if(!wfr_try_push(a->r,i,&ok))return(void*)1;}while(!ok);}return NULL;}
static void*c(void*x){A*a=x;for(int i=0;i<N;++i){int v;bool ok=false;do{if(!wfr_try_pop(a->r,&v,&ok))return(void*)1;}while(!ok);if(v!=i)return(void*)1;}return NULL;}
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){WaitFreeSpscRing*r=wfr_create(1025);if(!r||!wfr_platform_lock_free(r))return 1;A a={r};pthread_t pt,ct;struct timespec x,y;timespec_get(&x,TIME_UTC);if(pthread_create(&ct,NULL,c,&a)||pthread_create(&pt,NULL,p,&a))return 2;void*pr=NULL,*cr=NULL;pthread_join(pt,&pr);pthread_join(ct,&cr);timespec_get(&y,TIME_UTC);if(pr||cr)return 3;double s=e(x,y);printf("transfers=%d seconds=%.6f transfers_per_sec=%.0f\n",N,s,(double)N/s);wfr_free(r);return 0;}
