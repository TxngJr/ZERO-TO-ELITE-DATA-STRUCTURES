#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
typedef struct{int payload;atomic_bool ready;}Shared;
static void*producer(void*p){Shared*s=p;s->payload=42;atomic_store_explicit(&s->ready,true,memory_order_release);return NULL;}
static void*consumer(void*p){Shared*s=p;while(!atomic_load_explicit(&s->ready,memory_order_acquire)){}assert(s->payload==42);return NULL;}
int main(void){Shared s={.payload=0};atomic_init(&s.ready,false);pthread_t a,b;assert(pthread_create(&a,NULL,consumer,&s)==0);assert(pthread_create(&b,NULL,producer,&s)==0);assert(pthread_join(a,NULL)==0);assert(pthread_join(b,NULL)==0);puts("release/acquire publication passed");return 0;}
