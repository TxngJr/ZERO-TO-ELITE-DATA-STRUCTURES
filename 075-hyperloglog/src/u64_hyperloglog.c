#include "u64_hyperloglog.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

struct U64HyperLogLog {
    unsigned p;
    size_t m;
    uint8_t *registers;
};

static uint64_t mix64(uint64_t x){
    x^=x>>30;x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27;x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31;return x;
}

U64HyperLogLog *u64_hll_create(unsigned p){
    if(p<4U||p>18U)return NULL;
    U64HyperLogLog *h=calloc(1,sizeof *h);if(!h)return NULL;
    h->p=p;h->m=(size_t)1U<<p;
    h->registers=calloc(h->m,1);
    if(!h->registers){free(h);return NULL;}
    return h;
}

void u64_hll_free(U64HyperLogLog *h){if(!h)return;free(h->registers);free(h);}
unsigned u64_hll_precision(const U64HyperLogLog *h){return h?h->p:0;}
size_t u64_hll_register_count(const U64HyperLogLog *h){return h?h->m:0;}

bool u64_hll_add(U64HyperLogLog *h,uint64_t value){
    if(!h)return false;
    const uint64_t x=mix64(value^UINT64_C(0x9e3779b97f4a7c15));
    const size_t index=(size_t)(x>>(64U-h->p));
    const uint64_t w=x<<h->p;
    unsigned rank;
    if(w==0)rank=(64U-h->p)+1U;
    else rank=(unsigned)__builtin_clzll((unsigned long long)w)+1U;
    if(rank>h->registers[index])h->registers[index]=(uint8_t)rank;
    return true;
}

bool u64_hll_merge(U64HyperLogLog *out,const U64HyperLogLog *other){
    if(!out||!other||out->p!=other->p||out->m!=other->m)return false;
    for(size_t i=0;i<out->m;++i)if(other->registers[i]>out->registers[i])out->registers[i]=other->registers[i];
    return true;
}

static double alpha_for(size_t m){
    if(m==16)return 0.673;
    if(m==32)return 0.697;
    if(m==64)return 0.709;
    return 0.7213/(1.0+1.079/(double)m);
}

bool u64_hll_estimate(const U64HyperLogLog *h,double *out){
    if(!h||!out)return false;
    double sum=0.0;size_t zeros=0;
    for(size_t i=0;i<h->m;++i){
        sum+=ldexp(1.0,-(int)h->registers[i]);
        zeros+=h->registers[i]==0?1U:0U;
    }
    if(sum<=0.0)return false;
    const double m=(double)h->m;
    double estimate=alpha_for(h->m)*m*m/sum;

    if(estimate<=2.5*m&&zeros>0){
        estimate=m*log(m/(double)zeros);
    }

    const double two64=18446744073709551616.0;
    if(estimate>two64/30.0&&estimate<two64){
        estimate=-two64*log1p(-estimate/two64);
    }

    if(!isfinite(estimate)||estimate<0.0)return false;
    *out=estimate;return true;
}

bool u64_hll_validate(const U64HyperLogLog *h){
    if(!h||!h->registers||h->p<4U||h->p>18U||h->m!=((size_t)1U<<h->p))return false;
    const unsigned max_rank=(64U-h->p)+1U;
    for(size_t i=0;i<h->m;++i)if(h->registers[i]>max_rank)return false;
    return true;
}
