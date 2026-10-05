#include "u64_multi.h"
#include <stdlib.h>
#include <string.h>
typedef struct{uint64_t key;size_t count;}BagEntry;
typedef struct{uint64_t key;int64_t value;}Pair;
struct U64Multiset{BagEntry*data;size_t distinct,total,capacity;};
struct U64Multimap{Pair*data;size_t size,capacity;};
static size_t lb_bag(const U64Multiset*s,uint64_t k){size_t l=0,h=s->distinct;while(l<h){size_t m=l+(h-l)/2;if(s->data[m].key<k)l=m+1;else h=m;}return l;}
static bool reserve_bag(U64Multiset*s,size_t need){if(need<=s->capacity)return true;size_t c=s->capacity?s->capacity*2:8;if(c<need)c=need;if(c>SIZE_MAX/sizeof(BagEntry))return false;BagEntry*p=realloc(s->data,c*sizeof*p);if(!p)return false;s->data=p;s->capacity=c;return true;}
U64Multiset*u64_multiset_create(void){return calloc(1,sizeof(U64Multiset));}
void u64_multiset_free(U64Multiset*s){if(!s)return;free(s->data);free(s);}
size_t u64_multiset_distinct(const U64Multiset*s){return s?s->distinct:0;}
size_t u64_multiset_size(const U64Multiset*s){return s?s->total:0;}
bool u64_multiset_add(U64Multiset*s,uint64_t k,size_t c){if(!s)return false;if(c==0)return true;if(SIZE_MAX-s->total<c)return false;size_t i=lb_bag(s,k);if(i<s->distinct&&s->data[i].key==k){if(SIZE_MAX-s->data[i].count<c)return false;s->data[i].count+=c;s->total+=c;return true;}if(s->distinct==SIZE_MAX||!reserve_bag(s,s->distinct+1))return false;memmove(&s->data[i+1],&s->data[i],(s->distinct-i)*sizeof*s->data);s->data[i]=(BagEntry){k,c};++s->distinct;s->total+=c;return true;}
bool u64_multiset_remove(U64Multiset*s,uint64_t k,size_t c){if(!s||c==0)return c==0&&s!=NULL;size_t i=lb_bag(s,k);if(i>=s->distinct||s->data[i].key!=k||s->data[i].count<c)return false;s->data[i].count-=c;s->total-=c;if(s->data[i].count==0){memmove(&s->data[i],&s->data[i+1],(s->distinct-i-1)*sizeof*s->data);--s->distinct;}return true;}
size_t u64_multiset_count(const U64Multiset*s,uint64_t k){if(!s)return 0;size_t i=lb_bag(s,k);return i<s->distinct&&s->data[i].key==k?s->data[i].count:0;}
bool u64_multiset_nth_distinct(const U64Multiset*s,size_t i,uint64_t*k,size_t*c){if(!s||!k||!c||i>=s->distinct)return false;*k=s->data[i].key;*c=s->data[i].count;return true;}
bool u64_multiset_validate(const U64Multiset*s){if(!s||s->distinct>s->capacity||(s->capacity&&!s->data))return false;size_t sum=0;for(size_t i=0;i<s->distinct;++i){if(s->data[i].count==0)return false;if(i&&s->data[i-1].key>=s->data[i].key)return false;if(SIZE_MAX-sum<s->data[i].count)return false;sum+=s->data[i].count;}return sum==s->total;}
static bool reserve_map(U64Multimap*m,size_t need){if(need<=m->capacity)return true;size_t c=m->capacity?m->capacity*2:8;if(c<need)c=need;if(c>SIZE_MAX/sizeof(Pair))return false;Pair*p=realloc(m->data,c*sizeof*p);if(!p)return false;m->data=p;m->capacity=c;return true;}
static size_t lower_pair(const U64Multimap*m,uint64_t k){size_t l=0,h=m->size;while(l<h){size_t x=l+(h-l)/2;if(m->data[x].key<k)l=x+1;else h=x;}return l;}
static size_t upper_pair(const U64Multimap*m,uint64_t k){size_t l=0,h=m->size;while(l<h){size_t x=l+(h-l)/2;if(m->data[x].key<=k)l=x+1;else h=x;}return l;}
U64Multimap*u64_multimap_create(void){return calloc(1,sizeof(U64Multimap));}
void u64_multimap_free(U64Multimap*m){if(!m)return;free(m->data);free(m);}
size_t u64_multimap_size(const U64Multimap*m){return m?m->size:0;}
bool u64_multimap_add(U64Multimap*m,uint64_t k,int64_t v){if(!m||m->size==SIZE_MAX)return false;size_t i=upper_pair(m,k);if(!reserve_map(m,m->size+1))return false;memmove(&m->data[i+1],&m->data[i],(m->size-i)*sizeof*m->data);m->data[i]=(Pair){k,v};++m->size;return true;}
size_t u64_multimap_count(const U64Multimap*m,uint64_t k){if(!m)return 0;return upper_pair(m,k)-lower_pair(m,k);}
bool u64_multimap_get_nth(const U64Multimap*m,uint64_t k,size_t occ,int64_t*v){if(!m||!v)return false;size_t a=lower_pair(m,k),b=upper_pair(m,k);if(occ>=b-a)return false;*v=m->data[a+occ].value;return true;}
bool u64_multimap_remove_one(U64Multimap*m,uint64_t k,int64_t v){if(!m)return false;size_t a=lower_pair(m,k),b=upper_pair(m,k);for(size_t i=a;i<b;++i)if(m->data[i].value==v){memmove(&m->data[i],&m->data[i+1],(m->size-i-1)*sizeof*m->data);--m->size;return true;}return false;}
size_t u64_multimap_remove_all(U64Multimap*m,uint64_t k){if(!m)return 0;size_t a=lower_pair(m,k),b=upper_pair(m,k),n=b-a;if(n){memmove(&m->data[a],&m->data[b],(m->size-b)*sizeof*m->data);m->size-=n;}return n;}
bool u64_multimap_validate(const U64Multimap*m){if(!m||m->size>m->capacity||(m->capacity&&!m->data))return false;for(size_t i=1;i<m->size;++i)if(m->data[i-1].key>m->data[i].key)return false;return true;}
