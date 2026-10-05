#include "byte_counting_bloom.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static void encode(uint64_t value,uint8_t out[8]) {
    for(unsigned i=0;i<8;++i)out[i]=(uint8_t)(value>>(8U*i));
}

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static void test_add_remove(void) {
    enum { N=3000 };

    ByteCountingBloom *filter=byte_counting_bloom_create(
        65536,5
    );
    assert(filter!=NULL);

    uint8_t key[8];

    for(uint64_t i=0;i<N;++i) {
        encode(i,key);
        assert(byte_counting_bloom_add(filter,key,sizeof key));
    }

    for(uint64_t i=0;i<N;++i) {
        encode(i,key);
        bool maybe=false;
        assert(byte_counting_bloom_maybe_contains(
            filter,key,sizeof key,&maybe
        ));
        assert(maybe);
    }

    for(uint64_t i=0;i<N;i+=2) {
        encode(i,key);
        assert(byte_counting_bloom_remove(filter,key,sizeof key));
    }

    for(uint64_t i=1;i<N;i+=2) {
        encode(i,key);
        bool maybe=false;
        assert(byte_counting_bloom_maybe_contains(
            filter,key,sizeof key,&maybe
        ));
        assert(maybe);
    }

    assert(byte_counting_bloom_logical_count(filter)==N/2);
    assert(byte_counting_bloom_validate(filter));
    byte_counting_bloom_free(filter);
}

static void test_duplicates(void) {
    const uint8_t key[]={1,2,3,4};

    ByteCountingBloom *filter=byte_counting_bloom_create(4096,5);
    assert(filter!=NULL);

    assert(byte_counting_bloom_add(filter,key,sizeof key));
    assert(byte_counting_bloom_add(filter,key,sizeof key));
    assert(byte_counting_bloom_remove(filter,key,sizeof key));

    bool maybe=false;
    assert(byte_counting_bloom_maybe_contains(
        filter,key,sizeof key,&maybe
    ));
    assert(maybe);

    assert(byte_counting_bloom_remove(filter,key,sizeof key));
    assert(byte_counting_bloom_maybe_contains(
        filter,key,sizeof key,&maybe
    ));
    assert(!maybe);

    assert(byte_counting_bloom_validate(filter));
    byte_counting_bloom_free(filter);
}

static void test_saturation_rollback(void) {
    ByteCountingBloom *filter=byte_counting_bloom_create(1,1);
    assert(filter!=NULL);

    for(size_t i=0;i<UINT16_MAX;++i) {
        assert(byte_counting_bloom_add(filter,NULL,0));
    }

    assert(!byte_counting_bloom_add(filter,NULL,0));
    assert(byte_counting_bloom_logical_count(filter)==UINT16_MAX);
    assert(byte_counting_bloom_nonzero_counters(filter)==1);

    assert(byte_counting_bloom_remove(filter,NULL,0));
    assert(byte_counting_bloom_add(filter,NULL,0));
    assert(byte_counting_bloom_validate(filter));

    byte_counting_bloom_free(filter);
}

static void test_randomized_valid_deletes(void) {
    enum { KEYS=256, OPS=20000 };
    size_t multiplicity[KEYS]={0};

    ByteCountingBloom *filter=byte_counting_bloom_create(
        32768,5
    );
    assert(filter!=NULL);

    uint32_t rng=0x69A12345u;
    uint8_t key[8];

    for(int op=0;op<OPS;++op) {
        const size_t id=next_rng(&rng)%KEYS;
        encode((uint64_t)id,key);

        if((next_rng(&rng)&1U)!=0U||multiplicity[id]==0) {
            assert(byte_counting_bloom_add(filter,key,sizeof key));
            ++multiplicity[id];
        } else {
            assert(byte_counting_bloom_remove(filter,key,sizeof key));
            --multiplicity[id];
        }

        if((op%257)==0) {
            for(size_t k=0;k<KEYS;++k) {
                if(multiplicity[k]==0)continue;

                encode((uint64_t)k,key);
                bool maybe=false;
                assert(byte_counting_bloom_maybe_contains(
                    filter,key,sizeof key,&maybe
                ));
                assert(maybe);
            }

            assert(byte_counting_bloom_validate(filter));
        }
    }

    assert(byte_counting_bloom_validate(filter));
    byte_counting_bloom_free(filter);
}

int main(void) {
    assert(byte_counting_bloom_create(0,4)==NULL);
    assert(byte_counting_bloom_create(100,0)==NULL);

    test_add_remove();
    test_duplicates();
    test_saturation_rollback();
    test_randomized_valid_deletes();

    puts("Counting Bloom Filter tests passed");
    return 0;
}
