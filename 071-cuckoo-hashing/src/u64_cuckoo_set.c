#include "u64_cuckoo_set.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum { MAX_KICKS=256, MAX_REHASH_TRIES=16 };

struct U64CuckooSet {
    size_t capacity;
    size_t size;
    uint64_t seed1;
    uint64_t seed2;
    uint64_t *table1;
    uint64_t *table2;
    uint8_t *used1;
    uint8_t *used2;
};

static uint64_t mix64(uint64_t x){
    x^=x>>30;x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27;x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31;return x;
}

static size_t h1(const U64CuckooSet *s,uint64_t k){return (size_t)mix64(k^s->seed1)&(s->capacity-1);}
static size_t h2(const U64CuckooSet *s,uint64_t k){return (size_t)mix64(k^s->seed2)&(s->capacity-1);}

static bool alloc_arrays(U64CuckooSet *s,size_t cap){
    if(cap>SIZE_MAX/sizeof(uint64_t))return false;
    s->table1=calloc(cap,sizeof *s->table1);
    s->table2=calloc(cap,sizeof *s->table2);
    s->used1=calloc(cap,1);
    s->used2=calloc(cap,1);

    if(!s->table1||!s->table2||!s->used1||!s->used2){
        free(s->table1);free(s->table2);free(s->used1);free(s->used2);
        s->table1=s->table2=NULL;s->used1=s->used2=NULL;
        return false;
    }

    return true;
}

static void free_arrays(U64CuckooSet *s){
    free(s->table1);free(s->table2);free(s->used1);free(s->used2);
    s->table1=s->table2=NULL;s->used1=s->used2=NULL;
}

U64CuckooSet *u64_cuckoo_set_create(size_t cap){
    if(cap<4||(cap&(cap-1))!=0)return NULL;
    U64CuckooSet *s=calloc(1,sizeof *s);if(!s)return NULL;
    s->capacity=cap;s->seed1=UINT64_C(0x123456789abcdef0);s->seed2=UINT64_C(0xfedcba9876543210);
    if(!alloc_arrays(s,cap)){free_arrays(s);free(s);return NULL;}
    return s;
}

void u64_cuckoo_set_free(U64CuckooSet *s){if(!s)return;free_arrays(s);free(s);}
size_t u64_cuckoo_set_size(const U64CuckooSet *s){return s?s->size:0;}
size_t u64_cuckoo_set_table_capacity(const U64CuckooSet *s){return s?s->capacity:0;}

bool u64_cuckoo_set_contains(const U64CuckooSet *s,uint64_t key){
    if(!s)return false;
    size_t a=h1(s,key),b=h2(s,key);
    return (s->used1[a]&&s->table1[a]==key)||(s->used2[b]&&s->table2[b]==key);
}

static bool place_no_resize(U64CuckooSet *s,uint64_t key){
    if(u64_cuckoo_set_contains(s,key))return true;
    size_t i=h1(s,key);
    if(!s->used1[i]){s->used1[i]=1;s->table1[i]=key;++s->size;return true;}
    i=h2(s,key);
    if(!s->used2[i]){s->used2[i]=1;s->table2[i]=key;++s->size;return true;}

    uint64_t cur=key;
    unsigned table=1;
    for(size_t kick=0;kick<MAX_KICKS;++kick){
        if(table==1){
            i=h1(s,cur);
            uint64_t ev=s->table1[i];s->table1[i]=cur;cur=ev;
            i=h2(s,cur);
            if(!s->used2[i]){s->used2[i]=1;s->table2[i]=cur;++s->size;return true;}
            table=2;
        }else{
            i=h2(s,cur);
            uint64_t ev=s->table2[i];s->table2[i]=cur;cur=ev;
            i=h1(s,cur);
            if(!s->used1[i]){s->used1[i]=1;s->table1[i]=cur;++s->size;return true;}
            table=1;
        }
    }
    return false;
}

static bool rebuild_with(U64CuckooSet *s,uint64_t extra,bool has_extra,size_t newcap){
    const size_t total=s->size+(has_extra?1U:0U);
    if(total>SIZE_MAX/sizeof(uint64_t))return false;
    uint64_t *keys=malloc((total?total:1)*sizeof *keys);if(!keys)return false;
    size_t n=0;
    for(size_t i=0;i<s->capacity;++i){if(s->used1[i])keys[n++]=s->table1[i];if(s->used2[i])keys[n++]=s->table2[i];}
    if(has_extra)keys[n++]=extra;

    for(unsigned attempt=0;attempt<MAX_REHASH_TRIES;++attempt){
        U64CuckooSet temp={0};temp.capacity=newcap;
        temp.seed1=mix64(s->seed1+UINT64_C(0x9e3779b97f4a7c15)+(uint64_t)attempt);
        temp.seed2=mix64(s->seed2+UINT64_C(0xbf58476d1ce4e5b9)+(uint64_t)attempt);
        if(!alloc_arrays(&temp,newcap)){free(keys);return false;}
        bool ok=true;
        for(size_t i=0;i<n;++i){if(!place_no_resize(&temp,keys[i])){ok=false;break;}}
        if(ok){
            free_arrays(s);
            s->capacity=temp.capacity;s->size=temp.size;s->seed1=temp.seed1;s->seed2=temp.seed2;
            s->table1=temp.table1;s->table2=temp.table2;s->used1=temp.used1;s->used2=temp.used2;
            free(keys);return true;
        }
        free_arrays(&temp);
    }
    free(keys);return false;
}

bool u64_cuckoo_set_insert(U64CuckooSet *s,uint64_t key){
    if(!s)return false;
    if(u64_cuckoo_set_contains(s,key))return true;
    if(s->size+1>s->capacity){
        if(s->capacity>SIZE_MAX/2)return false;
        return rebuild_with(s,key,true,s->capacity*2);
    }

    const size_t cap=s->capacity;
    uint64_t *t1=malloc(cap*sizeof *t1),*t2=malloc(cap*sizeof *t2);
    uint8_t *u1=malloc(cap),*u2=malloc(cap);
    if(!t1||!t2||!u1||!u2){free(t1);free(t2);free(u1);free(u2);return false;}
    memcpy(t1,s->table1,cap*sizeof *t1);memcpy(t2,s->table2,cap*sizeof *t2);
    memcpy(u1,s->used1,cap);memcpy(u2,s->used2,cap);
    const size_t oldsize=s->size;

    if(place_no_resize(s,key)){free(t1);free(t2);free(u1);free(u2);return true;}

    memcpy(s->table1,t1,cap*sizeof *t1);memcpy(s->table2,t2,cap*sizeof *t2);
    memcpy(s->used1,u1,cap);memcpy(s->used2,u2,cap);s->size=oldsize;
    free(t1);free(t2);free(u1);free(u2);

    if(cap>SIZE_MAX/2)return false;
    return rebuild_with(s,key,true,cap*2);
}

bool u64_cuckoo_set_remove(U64CuckooSet *s,uint64_t key){
    if(!s)return false;
    size_t a=h1(s,key),b=h2(s,key);
    if(s->used1[a]&&s->table1[a]==key){s->used1[a]=0;--s->size;return true;}
    if(s->used2[b]&&s->table2[b]==key){s->used2[b]=0;--s->size;return true;}
    return false;
}

bool u64_cuckoo_set_validate(const U64CuckooSet *s){
    if(!s||!s->table1||!s->table2||!s->used1||!s->used2||s->capacity<4||(s->capacity&(s->capacity-1))!=0)return false;
    size_t count=0;
    for(size_t i=0;i<s->capacity;++i){
        if(s->used1[i]){if(h1(s,s->table1[i])!=i)return false;++count;}
        if(s->used2[i]){if(h2(s,s->table2[i])!=i)return false;++count;}
    }
    const size_t total_slots=
        s->capacity>SIZE_MAX/2?SIZE_MAX:2*s->capacity;

    return count==s->size&&s->size<=total_slots;
}
