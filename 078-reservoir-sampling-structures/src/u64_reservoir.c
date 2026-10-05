#include "u64_reservoir.h"

#include <stdint.h>
#include <stdlib.h>

struct U64Reservoir {
    size_t capacity;
    size_t sample_count;
    uint64_t seen;
    uint64_t rng;
    uint64_t *sample;
};

static uint64_t next_u64(U64Reservoir *r){
    uint64_t x=r->rng;
    x^=x>>12;x^=x<<25;x^=x>>27;
    r->rng=x;
    return x*UINT64_C(2685821657736338717);
}

static uint64_t uniform_bounded(U64Reservoir *r,uint64_t bound){
    const uint64_t threshold=(uint64_t)(-bound)%bound;
    for(;;){const uint64_t x=next_u64(r);if(x>=threshold)return x%bound;}
}

U64Reservoir *u64_reservoir_create(size_t capacity,uint64_t seed){
    if(capacity==0||capacity>SIZE_MAX/sizeof(uint64_t))return NULL;
    U64Reservoir *r=calloc(1,sizeof *r);if(!r)return NULL;
    r->capacity=capacity;
    r->rng=seed?seed:UINT64_C(0x9e3779b97f4a7c15);
    r->sample=malloc(capacity*sizeof *r->sample);
    if(!r->sample){free(r);return NULL;}
    return r;
}

void u64_reservoir_free(U64Reservoir *r){if(!r)return;free(r->sample);free(r);}
size_t u64_reservoir_capacity(const U64Reservoir *r){return r?r->capacity:0;}
size_t u64_reservoir_sample_count(const U64Reservoir *r){return r?r->sample_count:0;}
uint64_t u64_reservoir_seen(const U64Reservoir *r){return r?r->seen:0;}

bool u64_reservoir_add(U64Reservoir *r,uint64_t value){
    if(!r||r->seen==UINT64_MAX)return false;
    const uint64_t new_seen=r->seen+1U;

    if(r->sample_count<r->capacity){
        r->sample[r->sample_count++]=value;
    }else{
        const uint64_t j=uniform_bounded(r,new_seen);
        if(j<(uint64_t)r->capacity)r->sample[(size_t)j]=value;
    }

    r->seen=new_seen;
    return true;
}

bool u64_reservoir_get(const U64Reservoir *r,size_t index,uint64_t *out){
    if(!r||!out||index>=r->sample_count)return false;
    *out=r->sample[index];return true;
}

bool u64_reservoir_validate(const U64Reservoir *r){
    if(!r||!r->sample||r->capacity==0||r->sample_count>r->capacity)return false;
    const uint64_t expected=r->seen<(uint64_t)r->capacity?r->seen:(uint64_t)r->capacity;
    return r->sample_count==(size_t)expected;
}
