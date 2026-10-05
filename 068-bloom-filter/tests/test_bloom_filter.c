#include "byte_bloom_filter.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static void encode_u64(uint64_t value,uint8_t out[8]) {
    for(unsigned i=0;i<8;++i) {
        out[i]=(uint8_t)(value>>(8U*i));
    }
}

static void test_no_false_negatives(void) {
    enum { N=5000, NEG=10000 };

    ByteBloomFilter *filter=byte_bloom_filter_create(
        65536,5
    );
    assert(filter!=NULL);

    uint8_t key[8];

    for(uint64_t i=0;i<N;++i) {
        encode_u64(i,key);
        assert(byte_bloom_filter_add(filter,key,sizeof key));
    }

    assert(byte_bloom_filter_validate(filter));
    assert(byte_bloom_filter_insertions(filter)==N);

    for(uint64_t i=0;i<N;++i) {
        encode_u64(i,key);
        bool maybe=false;
        assert(byte_bloom_filter_maybe_contains(
            filter,key,sizeof key,&maybe
        ));
        assert(maybe);
    }

    size_t false_positives=0;

    for(uint64_t i=0;i<NEG;++i) {
        encode_u64(UINT64_C(1000000)+i,key);
        bool maybe=false;

        assert(byte_bloom_filter_maybe_contains(
            filter,key,sizeof key,&maybe
        ));

        false_positives+=maybe?1U:0U;
    }

    assert(false_positives<1000);
    byte_bloom_filter_free(filter);
}

static void test_binary_and_empty(void) {
    ByteBloomFilter *filter=byte_bloom_filter_create(1024,4);
    assert(filter!=NULL);

    const uint8_t binary[]={0,255,0,1,0};

    assert(byte_bloom_filter_add(filter,binary,sizeof binary));
    assert(byte_bloom_filter_add(filter,NULL,0));

    bool maybe=false;

    assert(byte_bloom_filter_maybe_contains(
        filter,binary,sizeof binary,&maybe
    ));
    assert(maybe);

    assert(byte_bloom_filter_maybe_contains(
        filter,NULL,0,&maybe
    ));
    assert(maybe);

    assert(byte_bloom_filter_validate(filter));
    byte_bloom_filter_free(filter);
}

int main(void) {
    assert(byte_bloom_filter_create(0,4)==NULL);
    assert(byte_bloom_filter_create(100,0)==NULL);

    test_no_false_negatives();
    test_binary_and_empty();

    puts("Bloom Filter tests passed");
    return 0;
}
