#include "byte_rolling_hash.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static size_t naive_count(
    const uint8_t *text,size_t n,
    const uint8_t *pattern,size_t m
) {
    size_t count=0;

    for(size_t i=0;i+m<=n;++i) {
        if(memcmp(text+i,pattern,m)==0)++count;
    }

    return count;
}

static size_t naive_lcp(
    const uint8_t *text,
    size_t a,size_t b,size_t max_length
) {
    size_t length=0;

    while(length<max_length&&
          text[a+length]==text[b+length]) {
        ++length;
    }

    return length;
}

static void test_banana(void) {
    const uint8_t text[]={'b','a','n','a','n','a'};

    ByteRollingHash *hash=
        byte_rolling_hash_create(text,sizeof text);

    assert(hash!=NULL);
    assert(byte_rolling_hash_validate(hash));

    bool equal=false;

    assert(byte_rolling_hash_equal(
        hash,1,4,3,6,&equal
    ));
    assert(equal);

    assert(byte_rolling_hash_equal(
        hash,0,3,1,4,&equal
    ));
    assert(!equal);

    size_t lcp=0;

    assert(byte_rolling_hash_lcp(
        hash,1,3,3,&lcp
    ));
    assert(lcp==3);

    const uint8_t pattern[]={'a','n','a'};
    size_t count=0;
    size_t positions[4];
    size_t written=0;

    assert(byte_rolling_hash_count(
        hash,pattern,sizeof pattern,&count
    ));
    assert(count==2);

    assert(byte_rolling_hash_report(
        hash,pattern,sizeof pattern,
        positions,4,&written
    ));

    assert(written==2);
    assert(positions[0]==1);
    assert(positions[1]==3);

    byte_rolling_hash_free(hash);
}

static void test_binary(void) {
    const uint8_t text[]={0,1,0,1,0,255,0};
    const uint8_t pattern[]={0,1};

    ByteRollingHash *hash=
        byte_rolling_hash_create(text,sizeof text);

    assert(hash!=NULL);
    assert(byte_rolling_hash_validate(hash));

    size_t count=0;

    assert(byte_rolling_hash_count(
        hash,pattern,sizeof pattern,&count
    ));

    assert(count==2);
    byte_rolling_hash_free(hash);
}

static void test_randomized(void) {
    enum { ROUNDS=120, N=220, QUERIES=180 };

    uint32_t rng=0x63A12345u;

    for(int round=0;round<ROUNDS;++round) {
        uint8_t text[N];

        for(size_t i=0;i<N;++i) {
            text[i]=(uint8_t)(next_rng(&rng)%10U);
        }

        ByteRollingHash *hash=
            byte_rolling_hash_create(text,N);

        assert(hash!=NULL);
        assert(byte_rolling_hash_validate(hash));

        for(int q=0;q<QUERIES;++q) {
            const size_t a=next_rng(&rng)%N;
            const size_t b=next_rng(&rng)%N;

            size_t max=N-a;
            if(N-b<max)max=N-b;

            const size_t len=
                max==0?0:(next_rng(&rng)%(max+1));

            bool equal=false;

            assert(byte_rolling_hash_equal(
                hash,a,a+len,b,b+len,&equal
            ));

            const bool expected_equal=
                len==0||memcmp(text+a,text+b,len)==0;

            assert(equal==expected_equal);

            size_t lcp=0;

            assert(byte_rolling_hash_lcp(
                hash,a,b,max,&lcp
            ));

            assert(lcp==naive_lcp(text,a,b,max));

            uint8_t pattern[12];
            const size_t m=1+
                (next_rng(&rng)%sizeof pattern);

            for(size_t i=0;i<m;++i) {
                pattern[i]=(uint8_t)(next_rng(&rng)%10U);
            }

            const size_t expected=
                naive_count(text,N,pattern,m);

            size_t actual=0;

            assert(byte_rolling_hash_count(
                hash,pattern,m,&actual
            ));

            assert(actual==expected);
            assert(byte_rolling_hash_contains(
                hash,pattern,m
            )==(expected>0));
        }

        byte_rolling_hash_free(hash);
    }
}

int main(void) {
    ByteRollingHash *empty=
        byte_rolling_hash_create(NULL,0);

    assert(empty!=NULL);
    assert(byte_rolling_hash_validate(empty));
    byte_rolling_hash_free(empty);

    test_banana();
    test_binary();
    test_randomized();

    puts("Rolling Hash tests passed");
    return 0;
}
