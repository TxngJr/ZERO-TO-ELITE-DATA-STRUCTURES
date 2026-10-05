#include "u64_ordered.h"
#include <stdlib.h>
#include <string.h>
typedef struct{uint64_t key;int64_t value;}Entry;
struct U64OrderedMap{Entry*data;size_t size,capacity;};
struct U64OrderedSet{uint64_t*data;size_t size,capacity;};
static bool reserve_map(U64OrderedMap*m,size_t need){if(need<=m->capacity)return true;size_t c=m->capacity?m->capacity*2:8;if(c<need)c=need;if(c>SIZE_MAX/sizeof(Entry))return false;Entry*p=realloc(m->data,c*sizeof*p);if(!p)return false;m->data=p;m->capacity=c;return true;}
static bool reserve_set(U64OrderedSet*s,size_t need){if(need<=s->capacity)return true;size_t c=s->capacity?s->capacity*2:8;if(c<need)c=need;if(c>SIZE_MAX/sizeof(uint64_t))return false;uint64_t*p=realloc(s->data,c*sizeof*p);if(!p)return false;s->data=p;s->capacity=c;return true;}
U64OrderedMap*u64_ordered_map_create(void){return calloc(1,sizeof(U64OrderedMap));}
void u64_ordered_map_free(U64OrderedMap*m){if(!m)return;free(m->data);free(m);}
size_t u64_ordered_map_size(const U64OrderedMap*m){return m?m->size:0;}
size_t u64_ordered_map_lower_bound(const U64OrderedMap*m,uint64_t k){if(!m)return 0;size_t l=0,h=m->size;while(l<h){size_t x=l+(h-l)/2;if(m->data[x].key<k)l=x+1;else h=x;}return l;}
bool u64_ordered_map_put(U64OrderedMap*m,uint64_t k,int64_t v){if(!m)return false;size_t i=u64_ordered_map_lower_bound(m,k);if(i<m->size&&m->data[i].key==k){m->data[i].value=v;return true;}if(m->size==SIZE_MAX||!reserve_map(m,m->size+1))return false;memmove(&m->data[i+1],&m->data[i],(m->size-i)*sizeof *m->data);m->data[i]=(Entry){k,v};++m->size;return true;}
bool u64_ordered_map_get(const U64OrderedMap*m,uint64_t k,int64_t*out){if(!m||!out)return false;size_t i=u64_ordered_map_lower_bound(m,k);if(i>=m->size||m->data[i].key!=k)return false;*out=m->data[i].value;return true;}
bool u64_ordered_map_remove(U64OrderedMap*m,uint64_t k){if(!m)return false;size_t i=u64_ordered_map_lower_bound(m,k);if(i>=m->size||m->data[i].key!=k)return false;memmove(&m->data[i],&m->data[i+1],(m->size-i-1)*sizeof *m->data);--m->size;return true;}
bool u64_ordered_map_nth(const U64OrderedMap*m,size_t i,uint64_t*k,int64_t*v){if(!m||!k||!v||i>=m->size)return false;*k=m->data[i].key;*v=m->data[i].value;return true;}
bool u64_ordered_map_validate(const U64OrderedMap*m){if(!m||m->size>m->capacity||(m->capacity&& !m->data))return false;for(size_t i=1;i<m->size;++i)if(m->data[i-1].key>=m->data[i].key)return false;return true;}
U64OrderedSet*u64_ordered_set_create(void){return calloc(1,sizeof(U64OrderedSet));}
void u64_ordered_set_free(U64OrderedSet*s){if(!s)return;free(s->data);free(s);}
size_t u64_ordered_set_size(const U64OrderedSet*s){return s?s->size:0;}
static size_t lbset(const U64OrderedSet*s,uint64_t k){size_t l=0,h=s->size;while(l<h){size_t x=l+(h-l)/2;if(s->data[x]<k)l=x+1;else h=x;}return l;}
bool u64_ordered_set_add(U64OrderedSet*s,uint64_t k){if(!s)return false;size_t i=lbset(s,k);if(i<s->size&&s->data[i]==k)return true;if(s->size==SIZE_MAX||!reserve_set(s,s->size+1))return false;memmove(&s->data[i+1],&s->data[i],(s->size-i)*sizeof*s->data);s->data[i]=k;++s->size;return true;}
bool u64_ordered_set_contains(const U64OrderedSet*s,uint64_t k){if(!s)return false;size_t i=lbset(s,k);return i<s->size&&s->data[i]==k;}
bool u64_ordered_set_remove(U64OrderedSet*s,uint64_t k){if(!s)return false;size_t i=lbset(s,k);if(i>=s->size||s->data[i]!=k)return false;memmove(&s->data[i],&s->data[i+1],(s->size-i-1)*sizeof*s->data);--s->size;return true;}
bool u64_ordered_set_nth(const U64OrderedSet*s,size_t i,uint64_t*k){if(!s||!k||i>=s->size)return false;*k=s->data[i];return true;}
bool u64_ordered_set_validate(const U64OrderedSet*s){if(!s||s->size>s->capacity||(s->capacity&&!s->data))return false;for(size_t i=1;i<s->size;++i)if(s->data[i-1]>=s->data[i])return false;return true;}
