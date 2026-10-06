#include "thread_safe_structures.h"
#include <pthread.h>
#include <stdio.h>
#include <time.h>
enum{P=4,C=4,N=100000};typedef struct{ThreadSafeQueue*q;int id;}PA;typedef struct{ThreadSafeQueue*q;}CA;
static void*prod(void*p){PA*a=p;for(int i=0;i<N;++i){bool ok=false;if(!tsq_push_wait(a->q,a->id*N+i,&ok)||!ok)return(void*)1;}return NULL;}
static void*cons(void*p){CA*a=p;for(;;){int v;bool ok=false;if(!tsq_pop_wait(a->q,&v,&ok))return(void*)1;if(!ok)break;}return NULL;}
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){ThreadSafeQueue*q=tsq_create(1024);if(!q)return 1;pthread_t p[P],c[C];PA pa[P];CA ca={q};struct timespec a,b;timespec_get(&a,TIME_UTC);for(int i=0;i<C;++i)if(pthread_create(&c[i],NULL,cons,&ca))return 2;for(int i=0;i<P;++i){pa[i]=(PA){q,i};if(pthread_create(&p[i],NULL,prod,&pa[i]))return 3;}for(int i=0;i<P;++i)pthread_join(p[i],NULL);tsq_close(q);for(int i=0;i<C;++i)pthread_join(c[i],NULL);timespec_get(&b,TIME_UTC);double s=e(a,b);printf("queue transfers=%d seconds=%.6f ops_per_sec=%.0f\n",P*N,s,(double)(P*N)/s);tsq_free(q);return 0;}
