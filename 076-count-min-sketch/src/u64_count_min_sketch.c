#include "u64_count_min_sketch.h"

#include <stdint.h>
#include <stdlib.h>

struct U64CountMinSketch {
    size_t width;
    size_t depth;
    uint64_t total_weight;
    uint64_t *counters;
    uint64_t *seeds;
};

static uint64_t mix64(uint64_t x){
    x^=x>>30;x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27;x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31;return x;
}

static size_t bucket(const U64CountMinSketch *s,size_t row,uint64_t key){
    return (size_t)(mix64(key^s->seeds[row])%(uint64_t)s->width);
}

U64CountMinSketch *u64_cms_create(size_t width,size_t depth){
    if(width==0||depth==0||width>UINT64_MAX)return NULL;
    if(depth>SIZE_MAX/width)return NULL;
    const size_t cells=width*depth;
    if(cells>SIZE_MAX/sizeof(uint64_t)||depth>SIZE_MAX/sizeof(uint64_t))return NULL;

    U64CountMinSketch *s=calloc(1,sizeof *s);
    if(!s)return NULL;
    s->width=width;s->depth=depth;
    s->counters=calloc(cells,sizeof *s->counters);
    s->seeds=malloc(depth*sizeof *s->seeds);
    if(!s->counters||!s->seeds){u64_cms_free(s);return NULL;}

    for(size_t r=0;r<depth;++r){
        s->seeds[r]=mix64(UINT64_C(0x9e3779b97f4a7c15)*(uint64_t)(r+1));
    }
    return s;
}

void u64_cms_free(U64CountMinSketch *s){if(!s)return;free(s->counters);free(s->seeds);free(s);}
size_t u64_cms_width(const U64CountMinSketch *s){return s?s->width:0;}
size_t u64_cms_depth(const U64CountMinSketch *s){return s?s->depth:0;}
uint64_t u64_cms_total_weight(const U64CountMinSketch *s){return s?s->total_weight:0;}

bool u64_cms_add(U64CountMinSketch *s,uint64_t key,uint64_t delta){
    if(!s)return false;
    if(delta==0)return true;
    if(UINT64_MAX-s->total_weight<delta)return false;

    for(size_t r=0;r<s->depth;++r){
        const size_t pos=r*s->width+bucket(s,r,key);
        if(UINT64_MAX-s->counters[pos]<delta)return false;
    }

    for(size_t r=0;r<s->depth;++r){
        const size_t pos=r*s->width+bucket(s,r,key);
        s->counters[pos]+=delta;
    }
    s->total_weight+=delta;
    return true;
}

bool u64_cms_estimate(const U64CountMinSketch *s,uint64_t key,uint64_t *out){
    if(!s||!out)return false;
    uint64_t best=UINT64_MAX;
    for(size_t r=0;r<s->depth;++r){
        const uint64_t v=s->counters[r*s->width+bucket(s,r,key)];
        if(v<best)best=v;
    }
    *out=best;
    return true;
}

bool u64_cms_merge(U64CountMinSketch *out,const U64CountMinSketch *other){
    if(!out||!other||out->width!=other->width||out->depth!=other->depth)return false;
    if(UINT64_MAX-out->total_weight<other->total_weight)return false;
    const size_t cells=out->width*out->depth;
    for(size_t i=0;i<cells;++i)if(UINT64_MAX-out->counters[i]<other->counters[i])return false;
    for(size_t i=0;i<cells;++i)out->counters[i]+=other->counters[i];
    out->total_weight+=other->total_weight;
    return true;
}

bool u64_cms_validate(const U64CountMinSketch *s){
    if(!s||!s->counters||!s->seeds||s->width==0||s->depth==0||s->width>UINT64_MAX)return false;
    for(size_t r=0;r<s->depth;++r){
        uint64_t row_sum=0;
        for(size_t c=0;c<s->width;++c){
            const uint64_t v=s->counters[r*s->width+c];
            if(UINT64_MAX-row_sum<v)return false;
            row_sum+=v;
        }
        if(row_sum!=s->total_weight)return false;
    }
    return true;
}
