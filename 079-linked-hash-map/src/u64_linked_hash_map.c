#include "u64_linked_hash_map.h"
#include <stdint.h>
#include <stdlib.h>
typedef struct Node{uint64_t key;int64_t value;struct Node *bucket_next,*order_prev,*order_next;}Node;
struct U64LinkedHashMap{Node **buckets;size_t bucket_count,size;Node *head,*tail;};
static uint64_t mix64(uint64_t x){x^=x>>30;x*=UINT64_C(0xbf58476d1ce4e5b9);x^=x>>27;x*=UINT64_C(0x94d049bb133111eb);x^=x>>31;return x;}
static size_t bi(const U64LinkedHashMap*m,uint64_t k){return (size_t)(mix64(k)%m->bucket_count);}
U64LinkedHashMap*u64_lhm_create(size_t n){if(n==0||n>SIZE_MAX/sizeof(Node*))return NULL;U64LinkedHashMap*m=calloc(1,sizeof*m);if(!m)return NULL;m->buckets=calloc(n,sizeof* m->buckets);if(!m->buckets){free(m);return NULL;}m->bucket_count=n;return m;}
void u64_lhm_free(U64LinkedHashMap*m){if(!m)return;Node*n=m->head;while(n){Node*x=n->order_next;free(n);n=x;}free(m->buckets);free(m);}
size_t u64_lhm_size(const U64LinkedHashMap*m){return m?m->size:0;}
static Node*find_node(const U64LinkedHashMap*m,uint64_t k){if(!m)return NULL;for(Node*n=m->buckets[bi(m,k)];n;n=n->bucket_next)if(n->key==k)return n;return NULL;}
static bool rehash(U64LinkedHashMap*m,size_t newc){if(newc==0||newc>SIZE_MAX/sizeof(Node*))return false;Node**b=calloc(newc,sizeof*b);if(!b)return false;for(Node*n=m->head;n;n=n->order_next){size_t i=(size_t)(mix64(n->key)%newc);n->bucket_next=b[i];b[i]=n;}free(m->buckets);m->buckets=b;m->bucket_count=newc;return true;}
bool u64_lhm_put(U64LinkedHashMap*m,uint64_t k,int64_t v){if(!m)return false;Node*n=find_node(m,k);if(n){n->value=v;return true;}if(m->size==SIZE_MAX)return false;if(m->size+1>m->bucket_count*3/4){if(m->bucket_count>SIZE_MAX/2)return false;if(!rehash(m,m->bucket_count*2))return false;}n=calloc(1,sizeof*n);if(!n)return false;n->key=k;n->value=v;size_t i=bi(m,k);n->bucket_next=m->buckets[i];m->buckets[i]=n;n->order_prev=m->tail;if(m->tail)m->tail->order_next=n;else m->head=n;m->tail=n;++m->size;return true;}
bool u64_lhm_get(const U64LinkedHashMap*m,uint64_t k,int64_t*out){if(!m||!out)return false;Node*n=find_node(m,k);if(!n)return false;*out=n->value;return true;}
bool u64_lhm_remove(U64LinkedHashMap*m,uint64_t k){if(!m)return false;size_t i=bi(m,k);Node**pp=&m->buckets[i];Node*n=*pp;while(n&&n->key!=k){pp=&n->bucket_next;n=n->bucket_next;}if(!n)return false;*pp=n->bucket_next;if(n->order_prev)n->order_prev->order_next=n->order_next;else m->head=n->order_next;if(n->order_next)n->order_next->order_prev=n->order_prev;else m->tail=n->order_prev;free(n);--m->size;return true;}
bool u64_lhm_nth(const U64LinkedHashMap*m,size_t idx,uint64_t*k,int64_t*v){if(!m||!k||!v||idx>=m->size)return false;Node*n=m->head;for(size_t i=0;i<idx;++i)n=n->order_next;*k=n->key;*v=n->value;return true;}
bool u64_lhm_validate(const U64LinkedHashMap*m){if(!m||!m->buckets||m->bucket_count==0)return false;size_t c=0;Node*prev=NULL;for(Node*n=m->head;n;n=n->order_next){if(n->order_prev!=prev)return false;if(bi(m,n->key)>=m->bucket_count)return false;bool found=false;for(Node*b=m->buckets[bi(m,n->key)];b;b=b->bucket_next)if(b==n){found=true;break;}if(!found)return false;prev=n;++c;}if(prev!=m->tail||c!=m->size)return false;size_t bc=0;for(size_t i=0;i<m->bucket_count;++i)for(Node*n=m->buckets[i];n;n=n->bucket_next){if(bi(m,n->key)!=i)return false;++bc;}return bc==m->size;}
