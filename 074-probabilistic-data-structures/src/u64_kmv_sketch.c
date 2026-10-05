#include "u64_kmv_sketch.h"

#include <float.h>
#include <stdint.h>
#include <stdlib.h>

struct U64KmvSketch {
    size_t k;
    size_t count;
    uint64_t *hashes;
};

static uint64_t mix64(uint64_t x){
    x^=x>>30;x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27;x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31;return x;
}

U64KmvSketch *u64_kmv_sketch_create(size_t k){
    if(k<2||k>SIZE_MAX/sizeof(uint64_t))return NULL;
    U64KmvSketch *s=calloc(1,sizeof *s);if(!s)return NULL;
    s->k=k;s->hashes=malloc(k*sizeof *s->hashes);
    if(!s->hashes){free(s);return NULL;}
    return s;
}

void u64_kmv_sketch_free(U64KmvSketch *s){if(!s)return;free(s->hashes);free(s);}
size_t u64_kmv_sketch_k(const U64KmvSketch *s){return s?s->k:0;}
size_t u64_kmv_sketch_retained(const U64KmvSketch *s){return s?s->count:0;}

static bool insert_hash(U64KmvSketch *s,uint64_t h){
    size_t max_i=0;
    for(size_t i=0;i<s->count;++i){
        if(s->hashes[i]==h)return true;
        if(s->hashes[i]>s->hashes[max_i])max_i=i;
    }
    if(s->count<s->k){s->hashes[s->count++]=h;return true;}
    if(h<s->hashes[max_i])s->hashes[max_i]=h;
    return true;
}

bool u64_kmv_sketch_add(U64KmvSketch *s,uint64_t value){
    if(!s)return false;
    return insert_hash(s,mix64(value));
}

bool u64_kmv_sketch_merge(U64KmvSketch *out,const U64KmvSketch *other){
    if(!out||!other||out->k!=other->k)return false;
    for(size_t i=0;i<other->count;++i)if(!insert_hash(out,other->hashes[i]))return false;
    return true;
}

bool u64_kmv_sketch_estimate(const U64KmvSketch *s,double *out){
    if(!s||!out)return false;
    if(s->count<s->k){*out=(double)s->count;return true;}
    uint64_t maxh=s->hashes[0];
    for(size_t i=1;i<s->count;++i)if(s->hashes[i]>maxh)maxh=s->hashes[i];
    if(maxh==0)return false;
    const long double universe=(long double)UINT64_MAX+1.0L;
    const long double estimate=((long double)(s->k-1)*universe)/(long double)maxh;
    if(estimate>(long double)DBL_MAX)return false;
    *out=(double)estimate;return true;
}

bool u64_kmv_sketch_validate(const U64KmvSketch *s){
    if(!s||s->k<2||!s->hashes||s->count>s->k)return false;
    for(size_t i=0;i<s->count;++i)for(size_t j=i+1;j<s->count;++j)if(s->hashes[i]==s->hashes[j])return false;
    return true;
}
