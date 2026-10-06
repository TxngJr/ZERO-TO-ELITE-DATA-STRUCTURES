#include "lock_free_stack.h"
#include <pthread.h>
#include <stdio.h>
#include <time.h>
enum{T=8,N=100000};typedef struct{LockFreeStack*s;int id;}A;
static void*w(void*p){A*a=p;for(int i=0;i<N;++i){bool x=false;if(!lfs_push(a->s,a->id*N+i,&x)||!x)return(void*)1;}return NULL;}
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){LockFreeStack*s=lfs_create((size_t)T*N);if(!s||!lfs_platform_lock_free(s))return 1;pthread_t t[T];A a[T];struct timespec x,y;timespec_get(&x,TIME_UTC);for(int i=0;i<T;++i){a[i]=(A){s,i};if(pthread_create(&t[i],NULL,w,&a[i]))return 2;}for(int i=0;i<T;++i)pthread_join(t[i],NULL);timespec_get(&y,TIME_UTC);double sec=e(x,y);printf("pushes=%d seconds=%.6f ops_per_sec=%.0f cas_failures=%zu\n",T*N,sec,(double)(T*N)/sec,lfs_push_cas_failures(s));lfs_free(s);return 0;}
