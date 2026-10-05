#include "int_bit_vector.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static size_t naive_rank1(
    const uint8_t *bits,
    size_t end
) {
    size_t c=0;
    for(size_t i=0;i<end;++i)c+=bits[i]!=0;
    return c;
}

static bool naive_select(
    const uint8_t *bits,
    size_t n,
    bool target,
    size_t k,
    size_t *out
) {
    size_t seen=0;

    for(size_t i=0;i<n;++i) {
        if((bits[i]!=0)==target) {
            if(seen==k) {
                *out=i;
                return true;
            }
            ++seen;
        }
    }

    return false;
}

static void test_known(void) {
    const uint8_t bits[]={0,1,0,1,1,0,1};

    IntBitVector *v=int_bit_vector_create(bits,sizeof bits);
    assert(v!=NULL);
    assert(int_bit_vector_validate(v));
    assert(int_bit_vector_ones(v)==4);
    assert(int_bit_vector_zeros(v)==3);

    const size_t expected_rank[]={0,0,1,1,2,3,3,4};

    for(size_t end=0;end<=7;++end) {
        size_t r1=0,r0=0;
        assert(int_bit_vector_rank1(v,end,&r1));
        assert(int_bit_vector_rank0(v,end,&r0));
        assert(r1==expected_rank[end]);
        assert(r0+r1==end);
    }

    const size_t ones[]={1,3,4,6};
    const size_t zeros[]={0,2,5};

    for(size_t k=0;k<4;++k) {
        size_t pos=0;
        assert(int_bit_vector_select1(v,k,&pos));
        assert(pos==ones[k]);
    }

    for(size_t k=0;k<3;++k) {
        size_t pos=0;
        assert(int_bit_vector_select0(v,k,&pos));
        assert(pos==zeros[k]);
    }

    int_bit_vector_free(v);
}

static void check_random_case(
    const uint8_t *bits,
    size_t n
) {
    IntBitVector *v=int_bit_vector_create(bits,n);
    assert(v!=NULL);
    assert(int_bit_vector_validate(v));

    const size_t ones=naive_rank1(bits,n);
    assert(int_bit_vector_ones(v)==ones);
    assert(int_bit_vector_zeros(v)==n-ones);

    for(size_t end=0;end<=n;++end) {
        size_t r1=0,r0=0;

        assert(int_bit_vector_rank1(v,end,&r1));
        assert(int_bit_vector_rank0(v,end,&r0));

        assert(r1==naive_rank1(bits,end));
        assert(r0==end-r1);
    }

    for(size_t k=0;k<ones;++k) {
        size_t expected=0,actual=0;
        assert(naive_select(bits,n,true,k,&expected));
        assert(int_bit_vector_select1(v,k,&actual));
        assert(actual==expected);
    }

    for(size_t k=0;k<n-ones;++k) {
        size_t expected=0,actual=0;
        assert(naive_select(bits,n,false,k,&expected));
        assert(int_bit_vector_select0(v,k,&actual));
        assert(actual==expected);
    }

    int_bit_vector_free(v);
}

static void test_boundaries(void) {
    const size_t sizes[]={1,63,64,65,127,128,129,130};
    uint8_t bits[130];

    for(size_t s=0;s<sizeof sizes/sizeof sizes[0];++s) {
        const size_t n=sizes[s];

        for(size_t i=0;i<n;++i) {
            bits[i]=(uint8_t)((i%3U)==0U);
        }

        check_random_case(bits,n);
    }
}

static void test_randomized(void) {
    enum { ROUNDS=140, MAX_N=320 };
    uint32_t rng=0x66A12345u;
    uint8_t bits[MAX_N];

    for(int round=0;round<ROUNDS;++round) {
        const size_t n=next_rng(&rng)%(MAX_N+1U);

        for(size_t i=0;i<n;++i) {
            bits[i]=(uint8_t)(next_rng(&rng)&1U);
        }

        check_random_case(bits,n);
    }
}

int main(void) {
    IntBitVector *invalid=
        int_bit_vector_create((const uint8_t[]){0,2,1},3);

    assert(invalid==NULL);

    test_known();
    test_boundaries();
    test_randomized();

    puts("Bit Vector tests passed");
    return 0;
}
