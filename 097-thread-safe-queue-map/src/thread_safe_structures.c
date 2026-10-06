#include "thread_safe_structures.h"
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

struct ThreadSafeQueue{pthread_mutex_t mu;pthread_cond_t not_empty,not_full;int*data;size_t capacity,head,tail,size;bool closed;};

ThreadSafeQueue*tsq_create(size_t capacity){
    if(capacity==0||capacity>SIZE_MAX/sizeof(int))return NULL;
    ThreadSafeQueue*q=calloc(1,sizeof*q);if(!q)return NULL;
    if(pthread_mutex_init(&q->mu,NULL)!=0){free(q);return NULL;}
    if(pthread_cond_init(&q->not_empty,NULL)!=0){pthread_mutex_destroy(&q->mu);free(q);return NULL;}
    if(pthread_cond_init(&q->not_full,NULL)!=0){pthread_cond_destroy(&q->not_empty);pthread_mutex_destroy(&q->mu);free(q);return NULL;}
    q->data=malloc(capacity*sizeof(int));
    if(!q->data){pthread_cond_destroy(&q->not_full);pthread_cond_destroy(&q->not_empty);pthread_mutex_destroy(&q->mu);free(q);return NULL;}
    q->capacity=capacity;return q;
}
void tsq_free(ThreadSafeQueue*q){if(!q)return;pthread_cond_destroy(&q->not_full);pthread_cond_destroy(&q->not_empty);pthread_mutex_destroy(&q->mu);free(q->data);free(q);}
static void push_locked(ThreadSafeQueue*q,int v){q->data[q->tail]=v;q->tail=(q->tail+1U)%q->capacity;++q->size;}
static int pop_locked(ThreadSafeQueue*q){int v=q->data[q->head];q->head=(q->head+1U)%q->capacity;--q->size;return v;}
bool tsq_try_push(ThreadSafeQueue*q,int v,bool*out){if(!q)return false;if(pthread_mutex_lock(&q->mu)!=0)return false;bool p=false;if(!q->closed&&q->size<q->capacity){push_locked(q,v);p=true;pthread_cond_signal(&q->not_empty);}if(pthread_mutex_unlock(&q->mu)!=0)return false;if(out)*out=p;return true;}
bool tsq_try_pop(ThreadSafeQueue*q,int*out_v,bool*out){if(!q||!out_v)return false;if(pthread_mutex_lock(&q->mu)!=0)return false;bool p=false;int v=0;if(q->size){v=pop_locked(q);p=true;pthread_cond_signal(&q->not_full);}if(pthread_mutex_unlock(&q->mu)!=0)return false;if(p)*out_v=v;if(out)*out=p;return true;}
bool tsq_push_wait(ThreadSafeQueue*q,int v,bool*out){if(!q)return false;if(pthread_mutex_lock(&q->mu)!=0)return false;int rc=0;while(!q->closed&&q->size==q->capacity&&rc==0)rc=pthread_cond_wait(&q->not_full,&q->mu);bool p=false;if(rc==0&&!q->closed){push_locked(q,v);p=true;pthread_cond_signal(&q->not_empty);}if(pthread_mutex_unlock(&q->mu)!=0)return false;if(rc)return false;if(out)*out=p;return true;}
bool tsq_pop_wait(ThreadSafeQueue*q,int*out_v,bool*out){if(!q||!out_v)return false;if(pthread_mutex_lock(&q->mu)!=0)return false;int rc=0;while(!q->closed&&q->size==0&&rc==0)rc=pthread_cond_wait(&q->not_empty,&q->mu);bool p=false;int v=0;if(rc==0&&q->size){v=pop_locked(q);p=true;pthread_cond_signal(&q->not_full);}if(pthread_mutex_unlock(&q->mu)!=0)return false;if(rc)return false;if(p)*out_v=v;if(out)*out=p;return true;}
bool tsq_close(ThreadSafeQueue*q){if(!q)return false;if(pthread_mutex_lock(&q->mu)!=0)return false;q->closed=true;int a=pthread_cond_broadcast(&q->not_empty),b=pthread_cond_broadcast(&q->not_full),c=pthread_mutex_unlock(&q->mu);return a==0&&b==0&&c==0;}
bool tsq_size(ThreadSafeQueue*q,size_t*out){if(!q||!out)return false;if(pthread_mutex_lock(&q->mu)!=0)return false;*out=q->size;return pthread_mutex_unlock(&q->mu)==0;}
bool tsq_validate_quiescent(ThreadSafeQueue*q){if(!q)return false;if(pthread_mutex_lock(&q->mu)!=0)return false;bool ok=q->data&&q->capacity>0&&q->size<=q->capacity&&q->head<q->capacity&&q->tail<q->capacity;if(ok)ok=((q->head+q->size)%q->capacity)==q->tail;if(pthread_mutex_unlock(&q->mu)!=0)return false;return ok;}

typedef struct MapNode{int key,value;struct MapNode*next;}MapNode;
typedef struct{pthread_mutex_t mu;MapNode*head;}MapBucket;
struct ThreadSafeIntMap{size_t bucket_count;MapBucket*buckets;pthread_mutex_t size_mu;size_t size;};
static uint64_t mix64(uint64_t x){x^=x>>30;x*=UINT64_C(0xbf58476d1ce4e5b9);x^=x>>27;x*=UINT64_C(0x94d049bb133111eb);x^=x>>31;return x;}
static size_t bucket_index(const ThreadSafeIntMap*m,int key){return(size_t)(mix64((uint64_t)(int64_t)key)%m->bucket_count);}
ThreadSafeIntMap*tsm_create(size_t n){
    if(n==0||n>SIZE_MAX/sizeof(MapBucket))return NULL;ThreadSafeIntMap*m=calloc(1,sizeof*m);if(!m)return NULL;
    m->buckets=calloc(n,sizeof(*m->buckets));if(!m->buckets){free(m);return NULL;}
    if(pthread_mutex_init(&m->size_mu,NULL)!=0){free(m->buckets);free(m);return NULL;}
    size_t i=0;for(;i<n;++i)if(pthread_mutex_init(&m->buckets[i].mu,NULL)!=0)break;
    if(i!=n){while(i>0)pthread_mutex_destroy(&m->buckets[--i].mu);pthread_mutex_destroy(&m->size_mu);free(m->buckets);free(m);return NULL;}m->bucket_count=n;return m;
}
void tsm_free(ThreadSafeIntMap*m){if(!m)return;for(size_t i=0;i<m->bucket_count;++i){MapNode*n=m->buckets[i].head;while(n){MapNode*x=n->next;free(n);n=x;}pthread_mutex_destroy(&m->buckets[i].mu);}pthread_mutex_destroy(&m->size_mu);free(m->buckets);free(m);}
bool tsm_put(ThreadSafeIntMap*m,int key,int value,bool*out){if(!m)return false;MapBucket*b=&m->buckets[bucket_index(m,key)];if(pthread_mutex_lock(&b->mu)!=0)return false;for(MapNode*n=b->head;n;n=n->next)if(n->key==key){n->value=value;if(pthread_mutex_unlock(&b->mu)!=0)return false;if(out)*out=false;return true;}MapNode*n=malloc(sizeof*n);if(!n){pthread_mutex_unlock(&b->mu);return false;}n->key=key;n->value=value;n->next=b->head;b->head=n;if(pthread_mutex_lock(&m->size_mu)!=0){b->head=n->next;free(n);pthread_mutex_unlock(&b->mu);return false;}++m->size;int a=pthread_mutex_unlock(&m->size_mu),c=pthread_mutex_unlock(&b->mu);if(a||c)return false;if(out)*out=true;return true;}
bool tsm_get(ThreadSafeIntMap*m,int key,int*out_v,bool*out_f){if(!m||!out_v||!out_f)return false;MapBucket*b=&m->buckets[bucket_index(m,key)];if(pthread_mutex_lock(&b->mu)!=0)return false;bool f=false;int v=0;for(MapNode*n=b->head;n;n=n->next)if(n->key==key){f=true;v=n->value;break;}if(pthread_mutex_unlock(&b->mu)!=0)return false;*out_f=f;if(f)*out_v=v;return true;}
bool tsm_remove(ThreadSafeIntMap*m,int key,bool*out){if(!m)return false;MapBucket*b=&m->buckets[bucket_index(m,key)];if(pthread_mutex_lock(&b->mu)!=0)return false;MapNode**link=&b->head;while(*link&&(*link)->key!=key)link=&(*link)->next;bool r=false;MapNode*dead=NULL;if(*link){dead=*link;*link=dead->next;r=true;if(pthread_mutex_lock(&m->size_mu)!=0){dead->next=*link;*link=dead;pthread_mutex_unlock(&b->mu);return false;}--m->size;if(pthread_mutex_unlock(&m->size_mu)!=0){pthread_mutex_unlock(&b->mu);return false;}}if(pthread_mutex_unlock(&b->mu)!=0)return false;free(dead);if(out)*out=r;return true;}
bool tsm_size(ThreadSafeIntMap*m,size_t*out){if(!m||!out)return false;if(pthread_mutex_lock(&m->size_mu)!=0)return false;*out=m->size;return pthread_mutex_unlock(&m->size_mu)==0;}
bool tsm_validate_quiescent(ThreadSafeIntMap*m){
    if(!m||!m->buckets||m->bucket_count==0)return false;size_t count=0;bool ok=true;
    for(size_t i=0;i<m->bucket_count&&ok;++i){if(pthread_mutex_lock(&m->buckets[i].mu)!=0)return false;for(MapNode*a=m->buckets[i].head;a;a=a->next){++count;for(MapNode*b=a->next;b;b=b->next)if(a->key==b->key){ok=false;break;}}if(pthread_mutex_unlock(&m->buckets[i].mu)!=0)return false;}
    if(!ok)return false;if(pthread_mutex_lock(&m->size_mu)!=0)return false;ok=count==m->size;if(pthread_mutex_unlock(&m->size_mu)!=0)return false;return ok;
}
