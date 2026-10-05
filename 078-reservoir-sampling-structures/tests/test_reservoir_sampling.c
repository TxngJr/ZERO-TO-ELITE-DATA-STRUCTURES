#include "u64_reservoir.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static void test_invariants(void){
    U64Reservoir *r=u64_reservoir_create(5,12345);assert(r);
    for(uint64_t i=0;i<3;++i){assert(u64_reservoir_add(r,i));assert(u64_reservoir_sample_count(r)==(size_t)(i+1));assert(u64_reservoir_validate(r));}
    for(uint64_t i=3;i<100;++i)assert(u64_reservoir_add(r,i));
    assert(u64_reservoir_seen(r)==100);assert(u64_reservoir_sample_count(r)==5);
    for(size_t i=0;i<5;++i){uint64_t v=0;assert(u64_reservoir_get(r,i,&v));assert(v<100);}
    assert(u64_reservoir_validate(r));u64_reservoir_free(r);
}

static void test_reproducibility(void){
    U64Reservoir *a=u64_reservoir_create(10,777),*b=u64_reservoir_create(10,777);assert(a&&b);
    for(uint64_t i=0;i<1000;++i){assert(u64_reservoir_add(a,i));assert(u64_reservoir_add(b,i));}
    for(size_t i=0;i<10;++i){uint64_t x=0,y=0;assert(u64_reservoir_get(a,i,&x));assert(u64_reservoir_get(b,i,&y));assert(x==y);}
    u64_reservoir_free(b);u64_reservoir_free(a);
}

static void test_uniformity_k1(void){
    enum { N=10, TRIALS=30000 };
    size_t counts[N]={0};
    for(uint64_t t=1;t<=TRIALS;++t){
        U64Reservoir *r=u64_reservoir_create(1,t*UINT64_C(0x9e3779b97f4a7c15));assert(r);
        for(uint64_t i=0;i<N;++i)assert(u64_reservoir_add(r,i));
        uint64_t v=0;assert(u64_reservoir_get(r,0,&v));assert(v<N);++counts[v];
        u64_reservoir_free(r);
    }
    const size_t expected=TRIALS/N;
    for(size_t i=0;i<N;++i){
        assert(counts[i]>expected*80/100);
        assert(counts[i]<expected*120/100);
    }
}

int main(void){
    assert(u64_reservoir_create(0,1)==NULL);
    test_invariants();test_reproducibility();test_uniformity_k1();
    puts("Reservoir Sampling tests passed");return 0;
}
