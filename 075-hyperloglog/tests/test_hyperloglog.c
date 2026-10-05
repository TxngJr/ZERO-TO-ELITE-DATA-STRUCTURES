#include "u64_hyperloglog.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

static double relerr(double e,double n){return fabs(e-n)/n;}

int main(void){
    assert(u64_hll_create(3)==NULL);assert(u64_hll_create(19)==NULL);
    U64HyperLogLog *empty=u64_hll_create(12);assert(empty);double e=1;assert(u64_hll_estimate(empty,&e));assert(e==0.0);assert(u64_hll_validate(empty));u64_hll_free(empty);

    U64HyperLogLog *small=u64_hll_create(12);assert(small);
    for(uint64_t i=0;i<100;++i){assert(u64_hll_add(small,i));assert(u64_hll_add(small,i));}
    assert(u64_hll_estimate(small,&e));assert(relerr(e,100.0)<0.10);u64_hll_free(small);

    const uint64_t n=100000;
    U64HyperLogLog *all=u64_hll_create(14),*a=u64_hll_create(14),*b=u64_hll_create(14);assert(all&&a&&b);
    for(uint64_t i=0;i<n;++i){assert(u64_hll_add(all,i));if(i<n/2)assert(u64_hll_add(a,i));else assert(u64_hll_add(b,i));}
    assert(u64_hll_estimate(all,&e));assert(relerr(e,(double)n)<0.03);
    assert(u64_hll_merge(a,b));double merged=0;assert(u64_hll_estimate(a,&merged));
    assert(fabs(merged-e)<1e-9);assert(u64_hll_validate(a));
    u64_hll_free(b);u64_hll_free(a);u64_hll_free(all);
    puts("HyperLogLog tests passed");return 0;
}
