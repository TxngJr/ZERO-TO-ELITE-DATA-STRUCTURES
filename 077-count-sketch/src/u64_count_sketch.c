#include "u64_count_sketch.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

struct U64CountSketch {
    size_t width;
    size_t depth;
    int64_t *counters;
    uint64_t *bucket_seeds;
    uint64_t *sign_seeds;
};

static uint64_t mix64(uint64_t x){
    x^=x>>30;x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27;x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31;return x;
}

static size_t bucket(const U64CountSketch *s,size_t row,uint64_t key){
    return (size_t)(mix64(key^s->bucket_seeds[row])%(uint64_t)s->width);
}

static int sign_for(const U64CountSketch *s,size_t row,uint64_t key){
    return (mix64(key^s->sign_seeds[row])&UINT64_C(1))?1:-1;
}

static bool safe_add_i64(int64_t a,int64_t b,int64_t *out){
    if((b>0&&a>INT64_MAX-b)||(b<0&&a<INT64_MIN-b))return false;
    *out=a+b;return true;
}

U64CountSketch *u64_count_sketch_create(size_t width,size_t depth){
    if(width==0||depth==0||(depth%2U)==0||width>UINT64_MAX)return NULL;
    if(depth>SIZE_MAX/width)return NULL;
    const size_t cells=width*depth;
    if(cells>SIZE_MAX/sizeof(int64_t)||depth>SIZE_MAX/sizeof(uint64_t))return NULL;

    U64CountSketch *s=calloc(1,sizeof *s);if(!s)return NULL;
    s->width=width;s->depth=depth;
    s->counters=calloc(cells,sizeof *s->counters);
    s->bucket_seeds=malloc(depth*sizeof *s->bucket_seeds);
    s->sign_seeds=malloc(depth*sizeof *s->sign_seeds);
    if(!s->counters||!s->bucket_seeds||!s->sign_seeds){u64_count_sketch_free(s);return NULL;}

    for(size_t r=0;r<depth;++r){
        s->bucket_seeds[r]=mix64(UINT64_C(0x9e3779b97f4a7c15)*(uint64_t)(r+1));
        s->sign_seeds[r]=mix64(UINT64_C(0xbf58476d1ce4e5b9)*(uint64_t)(r+17));
    }
    return s;
}

void u64_count_sketch_free(U64CountSketch *s){if(!s)return;free(s->counters);free(s->bucket_seeds);free(s->sign_seeds);free(s);}
size_t u64_count_sketch_width(const U64CountSketch *s){return s?s->width:0;}
size_t u64_count_sketch_depth(const U64CountSketch *s){return s?s->depth:0;}

bool u64_count_sketch_update(U64CountSketch *s,uint64_t key,int64_t delta){
    if(!s||delta==INT64_MIN)return false;
    if(delta==0)return true;

    for(size_t r=0;r<s->depth;++r){
        const size_t pos=r*s->width+bucket(s,r,key);
        const int sign=sign_for(s,r,key);
        const int64_t contribution=sign>0?delta:-delta;
        int64_t ignored=0;
        if(!safe_add_i64(s->counters[pos],contribution,&ignored))return false;
    }

    for(size_t r=0;r<s->depth;++r){
        const size_t pos=r*s->width+bucket(s,r,key);
        const int sign=sign_for(s,r,key);
        const int64_t contribution=sign>0?delta:-delta;
        s->counters[pos]+=contribution;
    }
    return true;
}

static void insertion_sort(int64_t *values,size_t n){
    for(size_t i=1;i<n;++i){
        const int64_t x=values[i];size_t j=i;
        while(j>0&&values[j-1]>x){values[j]=values[j-1];--j;}
        values[j]=x;
    }
}

bool u64_count_sketch_estimate(const U64CountSketch *s,uint64_t key,int64_t *out){
    if(!s||!out)return false;
    if(s->depth>SIZE_MAX/sizeof(int64_t))return false;
    int64_t *estimates=malloc(s->depth*sizeof *estimates);if(!estimates)return false;

    for(size_t r=0;r<s->depth;++r){
        const int64_t c=s->counters[r*s->width+bucket(s,r,key)];
        const int sign=sign_for(s,r,key);
        if(sign<0&&c==INT64_MIN){free(estimates);return false;}
        estimates[r]=sign>0?c:-c;
    }

    insertion_sort(estimates,s->depth);
    *out=estimates[s->depth/2U];
    free(estimates);return true;
}

bool u64_count_sketch_merge(U64CountSketch *out,const U64CountSketch *other){
    if(!out||!other||out->width!=other->width||out->depth!=other->depth)return false;
    const size_t cells=out->width*out->depth;
    for(size_t i=0;i<cells;++i){int64_t ignored=0;if(!safe_add_i64(out->counters[i],other->counters[i],&ignored))return false;}
    for(size_t i=0;i<cells;++i)out->counters[i]+=other->counters[i];
    return true;
}

bool u64_count_sketch_validate(const U64CountSketch *s){
    if(!s||!s->counters||!s->bucket_seeds||!s->sign_seeds||s->width==0||s->depth==0||(s->depth%2U)==0||s->width>UINT64_MAX)return false;
    return true;
}
