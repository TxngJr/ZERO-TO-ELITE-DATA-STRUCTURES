#include "byte_bloom_filter.h"

#include <stdint.h>
#include <stdlib.h>

struct ByteBloomFilter {
    size_t bit_count;
    size_t word_count;
    size_t hash_count;
    size_t insertions;
    uint64_t *words;
};

static uint64_t mix64(uint64_t x) {
    x^=x>>30;
    x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27;
    x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31;
    return x;
}

static uint64_t hash_bytes(
    const uint8_t *key,
    size_t length,
    uint64_t seed
) {
    uint64_t h=UINT64_C(1469598103934665603)^seed;

    for(size_t i=0;i<length;++i) {
        h^=(uint64_t)key[i];
        h*=UINT64_C(1099511628211);
        h=mix64(h+seed+(uint64_t)i);
    }

    h^=(uint64_t)length+seed;
    return mix64(h);
}

static void two_hashes(
    const uint8_t *key,
    size_t length,
    uint64_t *h1,
    uint64_t *h2
) {
    *h1=hash_bytes(key,length,UINT64_C(0x243f6a8885a308d3));
    *h2=hash_bytes(key,length,UINT64_C(0x13198a2e03707344));

    *h2|=UINT64_C(1);
}

static size_t words_for(size_t bits) {
    return bits/64U+(bits%64U!=0U?1U:0U);
}

static uint64_t final_mask(const ByteBloomFilter *filter) {
    const unsigned used=(unsigned)(filter->bit_count%64U);
    if(used==0U)return UINT64_MAX;
    return (UINT64_C(1)<<used)-UINT64_C(1);
}

static size_t bit_index(
    const ByteBloomFilter *filter,
    uint64_t h1,
    uint64_t h2,
    size_t i
) {
    const uint64_t combined=
        h1+(uint64_t)i*h2;

    return (size_t)(combined%(uint64_t)filter->bit_count);
}

ByteBloomFilter *byte_bloom_filter_create(
    size_t bit_count,
    size_t hash_count
) {
    if(bit_count==0||hash_count==0||
       bit_count>UINT64_MAX) {
        return NULL;
    }

    ByteBloomFilter *filter=calloc(1,sizeof *filter);
    if(filter==NULL)return NULL;

    filter->bit_count=bit_count;
    filter->word_count=words_for(bit_count);
    filter->hash_count=hash_count;

    if(filter->word_count>SIZE_MAX/sizeof *filter->words) {
        free(filter);
        return NULL;
    }

    filter->words=calloc(
        filter->word_count,
        sizeof *filter->words
    );

    if(filter->words==NULL) {
        free(filter);
        return NULL;
    }

    return filter;
}

void byte_bloom_filter_free(ByteBloomFilter *filter) {
    if(filter==NULL)return;
    free(filter->words);
    free(filter);
}

size_t byte_bloom_filter_bit_count(const ByteBloomFilter *filter) {
    return filter==NULL?0:filter->bit_count;
}

size_t byte_bloom_filter_hash_count(const ByteBloomFilter *filter) {
    return filter==NULL?0:filter->hash_count;
}

size_t byte_bloom_filter_insertions(const ByteBloomFilter *filter) {
    return filter==NULL?0:filter->insertions;
}

size_t byte_bloom_filter_set_bits(const ByteBloomFilter *filter) {
    if(filter==NULL)return 0;

    size_t total=0;

    for(size_t i=0;i<filter->word_count;++i) {
        total+=(size_t)__builtin_popcountll(
            (unsigned long long)filter->words[i]
        );
    }

    return total;
}

bool byte_bloom_filter_add(
    ByteBloomFilter *filter,
    const uint8_t *key,
    size_t key_length
) {
    if(filter==NULL||
       (key_length>0&&key==NULL)||
       filter->insertions==SIZE_MAX) {
        return false;
    }

    uint64_t h1=0,h2=0;
    two_hashes(key,key_length,&h1,&h2);

    for(size_t i=0;i<filter->hash_count;++i) {
        const size_t bit=bit_index(filter,h1,h2,i);
        filter->words[bit/64U]|=
            UINT64_C(1)<<(bit%64U);
    }

    ++filter->insertions;
    return true;
}

bool byte_bloom_filter_maybe_contains(
    const ByteBloomFilter *filter,
    const uint8_t *key,
    size_t key_length,
    bool *out_maybe
) {
    if(filter==NULL||out_maybe==NULL||
       (key_length>0&&key==NULL)) {
        return false;
    }

    uint64_t h1=0,h2=0;
    two_hashes(key,key_length,&h1,&h2);

    for(size_t i=0;i<filter->hash_count;++i) {
        const size_t bit=bit_index(filter,h1,h2,i);

        if((filter->words[bit/64U]&
            (UINT64_C(1)<<(bit%64U)))==0) {
            *out_maybe=false;
            return true;
        }
    }

    *out_maybe=true;
    return true;
}

bool byte_bloom_filter_validate(const ByteBloomFilter *filter) {
    if(filter==NULL||filter->bit_count==0||
       filter->hash_count==0||filter->word_count==0||
       filter->words==NULL||
       filter->word_count!=words_for(filter->bit_count)) {
        return false;
    }

    if((filter->words[filter->word_count-1]&
        ~final_mask(filter))!=0) {
        return false;
    }

    return byte_bloom_filter_set_bits(filter)<=filter->bit_count;
}
