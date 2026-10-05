#include "u64_count_min_sketch.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *s){*s=*s*1664525u+1013904223u;return *s;}

int main(void){
    assert(u64_cms_create(0,4)==NULL);
    U64CountMinSketch *s=u64_cms_create(2048,5);assert(s);
    enum { KEYS=512, OPS=50000 };
    uint64_t exact[KEYS];memset(exact,0,sizeof exact);
    uint32_t rng=0x76A12345u;

    for(int i=0;i<OPS;++i){
        const uint64_t key=next_rng(&rng)%KEYS;
        const uint64_t delta=1U+(next_rng(&rng)%5U);
        assert(u64_cms_add(s,key,delta));exact[key]+=delta;
    }

    uint64_t max_error=0;
    for(uint64_t key=0;key<KEYS;++key){
        uint64_t est=0;assert(u64_cms_estimate(s,key,&est));
        assert(est>=exact[key]);
        if(est-exact[key]>max_error)max_error=est-exact[key];
    }
    assert(max_error<100);
    assert(u64_cms_validate(s));

    U64CountMinSketch *a=u64_cms_create(2048,5),*b=u64_cms_create(2048,5);assert(a&&b);
    for(uint64_t i=0;i<10000;++i){if(i&1U)assert(u64_cms_add(a,i%100,1));else assert(u64_cms_add(b,i%100,1));}
    assert(u64_cms_merge(a,b));
    for(uint64_t k=0;k<100;++k){uint64_t est=0;assert(u64_cms_estimate(a,k,&est));assert(est>=100);}
    assert(u64_cms_validate(a));

    u64_cms_free(b);u64_cms_free(a);u64_cms_free(s);
    puts("Count-Min Sketch tests passed");return 0;
}
