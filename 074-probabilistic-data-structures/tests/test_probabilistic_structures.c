#include "u64_kmv_sketch.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

static double relerr(double estimate,double truth){return fabs(estimate-truth)/truth;}

int main(void){
    U64KmvSketch *small=u64_kmv_sketch_create(128);assert(small);
    for(uint64_t i=0;i<100;++i){assert(u64_kmv_sketch_add(small,i));assert(u64_kmv_sketch_add(small,i));}
    double est=0;assert(u64_kmv_sketch_estimate(small,&est));assert(est==100.0);assert(u64_kmv_sketch_validate(small));u64_kmv_sketch_free(small);

    U64KmvSketch *a=u64_kmv_sketch_create(1024);
    U64KmvSketch *b=u64_kmv_sketch_create(1024);
    U64KmvSketch *all=u64_kmv_sketch_create(1024);
    assert(a&&b&&all);
    const uint64_t n=50000;
    for(uint64_t i=0;i<n;++i){assert(u64_kmv_sketch_add(all,i));if(i<n/2)assert(u64_kmv_sketch_add(a,i));else assert(u64_kmv_sketch_add(b,i));}
    assert(u64_kmv_sketch_estimate(all,&est));
    assert(relerr(est,(double)n)<0.10);
    assert(u64_kmv_sketch_merge(a,b));
    double merged=0;assert(u64_kmv_sketch_estimate(a,&merged));
    assert(relerr(merged,(double)n)<0.10);
    assert(fabs(merged-est)/est<0.0000001);
    assert(u64_kmv_sketch_validate(a));
    u64_kmv_sketch_free(all);u64_kmv_sketch_free(b);u64_kmv_sketch_free(a);
    puts("Probabilistic Data Structures tests passed");return 0;
}
