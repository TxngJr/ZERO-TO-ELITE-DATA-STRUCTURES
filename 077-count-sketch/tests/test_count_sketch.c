#include "u64_count_sketch.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t next_rng(uint32_t *s){*s=*s*1664525u+1013904223u;return *s;}
static uint64_t absdiff(int64_t a,int64_t b){return a>=b?(uint64_t)(a-b):(uint64_t)(b-a);}

int main(void){
    assert(u64_count_sketch_create(100,4)==NULL);
    enum { KEYS=256, OPS=50000 };
    U64CountSketch *s=u64_count_sketch_create(16384,5);assert(s);
    int64_t exact[KEYS]={0};uint32_t rng=0x77A12345u;

    for(int i=0;i<OPS;++i){
        const uint64_t key=next_rng(&rng)%KEYS;
        const int64_t delta=(next_rng(&rng)&1U)?1:-1;
        assert(u64_count_sketch_update(s,key,delta));exact[key]+=delta;
    }

    uint64_t worst=0;
    for(uint64_t k=0;k<KEYS;++k){int64_t e=0;assert(u64_count_sketch_estimate(s,k,&e));const uint64_t d=absdiff(e,exact[k]);if(d>worst)worst=d;}
    assert(worst<30);assert(u64_count_sketch_validate(s));

    U64CountSketch *a=u64_count_sketch_create(4096,5),*b=u64_count_sketch_create(4096,5);assert(a&&b);
    for(uint64_t i=0;i<20000;++i){if(i&1U)assert(u64_count_sketch_update(a,i%100,1));else assert(u64_count_sketch_update(b,i%100,1));}
    assert(u64_count_sketch_merge(a,b));
    for(uint64_t k=0;k<100;++k){int64_t e=0;assert(u64_count_sketch_estimate(a,k,&e));assert(absdiff(e,200)<20);}

    assert(u64_count_sketch_update(a,42,-100));
    int64_t e=0;assert(u64_count_sketch_estimate(a,42,&e));assert(absdiff(e,100)<20);

    u64_count_sketch_free(b);u64_count_sketch_free(a);u64_count_sketch_free(s);
    puts("Count Sketch tests passed");return 0;
}
