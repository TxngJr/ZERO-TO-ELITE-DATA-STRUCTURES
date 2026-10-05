#include "byte_counting_bloom.h"

#include <stdint.h>
#include <stdlib.h>

struct ByteCountingBloom {
    size_t counter_count;
    size_t hash_count;
    size_t logical_count;
    size_t nonzero_count;
    uint16_t *counters;
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

static void hashes(
    const uint8_t *key,
    size_t length,
    uint64_t *h1,
    uint64_t *h2
) {
    *h1=hash_bytes(key,length,UINT64_C(0x243f6a8885a308d3));
    *h2=hash_bytes(key,length,UINT64_C(0x13198a2e03707344));
    *h2|=UINT64_C(1);
}

static size_t position(
    const ByteCountingBloom *filter,
    uint64_t h1,
    uint64_t h2,
    size_t i
) {
    return (size_t)(
        (h1+(uint64_t)i*h2)%
        (uint64_t)filter->counter_count
    );
}

ByteCountingBloom *byte_counting_bloom_create(
    size_t counter_count,
    size_t hash_count
) {
    if(counter_count==0||hash_count==0||
       counter_count>UINT64_MAX||
       counter_count>SIZE_MAX/sizeof(uint16_t)) {
        return NULL;
    }

    ByteCountingBloom *filter=calloc(1,sizeof *filter);
    if(filter==NULL)return NULL;

    filter->counter_count=counter_count;
    filter->hash_count=hash_count;
    filter->counters=calloc(counter_count,sizeof *filter->counters);

    if(filter->counters==NULL) {
        free(filter);
        return NULL;
    }

    return filter;
}

void byte_counting_bloom_free(ByteCountingBloom *filter) {
    if(filter==NULL)return;
    free(filter->counters);
    free(filter);
}

size_t byte_counting_bloom_counter_count(const ByteCountingBloom *filter) {
    return filter==NULL?0:filter->counter_count;
}

size_t byte_counting_bloom_hash_count(const ByteCountingBloom *filter) {
    return filter==NULL?0:filter->hash_count;
}

size_t byte_counting_bloom_logical_count(const ByteCountingBloom *filter) {
    return filter==NULL?0:filter->logical_count;
}

size_t byte_counting_bloom_nonzero_counters(const ByteCountingBloom *filter) {
    return filter==NULL?0:filter->nonzero_count;
}

static void rollback_increments(
    ByteCountingBloom *filter,
    uint64_t h1,
    uint64_t h2,
    size_t applied
) {
    for(size_t j=0;j<applied;++j) {
        const size_t p=position(filter,h1,h2,j);

        if(filter->counters[p]==1U) {
            --filter->nonzero_count;
        }

        --filter->counters[p];
    }
}

bool byte_counting_bloom_add(
    ByteCountingBloom *filter,
    const uint8_t *key,
    size_t key_length
) {
    if(filter==NULL||
       (key_length>0&&key==NULL)||
       filter->logical_count==SIZE_MAX) {
        return false;
    }

    uint64_t h1=0,h2=0;
    hashes(key,key_length,&h1,&h2);

    for(size_t i=0;i<filter->hash_count;++i) {
        const size_t p=position(filter,h1,h2,i);

        if(filter->counters[p]==UINT16_MAX) {
            rollback_increments(filter,h1,h2,i);
            return false;
        }

        if(filter->counters[p]==0U) {
            ++filter->nonzero_count;
        }

        ++filter->counters[p];
    }

    ++filter->logical_count;
    return true;
}

static void rollback_decrements(
    ByteCountingBloom *filter,
    uint64_t h1,
    uint64_t h2,
    size_t applied
) {
    for(size_t j=0;j<applied;++j) {
        const size_t p=position(filter,h1,h2,j);

        if(filter->counters[p]==0U) {
            ++filter->nonzero_count;
        }

        ++filter->counters[p];
    }
}

bool byte_counting_bloom_remove(
    ByteCountingBloom *filter,
    const uint8_t *key,
    size_t key_length
) {
    if(filter==NULL||
       (key_length>0&&key==NULL)||
       filter->logical_count==0) {
        return false;
    }

    uint64_t h1=0,h2=0;
    hashes(key,key_length,&h1,&h2);

    for(size_t i=0;i<filter->hash_count;++i) {
        const size_t p=position(filter,h1,h2,i);

        if(filter->counters[p]==0U) {
            rollback_decrements(filter,h1,h2,i);
            return false;
        }

        --filter->counters[p];

        if(filter->counters[p]==0U) {
            --filter->nonzero_count;
        }
    }

    --filter->logical_count;
    return true;
}

bool byte_counting_bloom_maybe_contains(
    const ByteCountingBloom *filter,
    const uint8_t *key,
    size_t key_length,
    bool *out_maybe
) {
    if(filter==NULL||out_maybe==NULL||
       (key_length>0&&key==NULL)) {
        return false;
    }

    uint64_t h1=0,h2=0;
    hashes(key,key_length,&h1,&h2);

    for(size_t i=0;i<filter->hash_count;++i) {
        const size_t p=position(filter,h1,h2,i);

        if(filter->counters[p]==0U) {
            *out_maybe=false;
            return true;
        }
    }

    *out_maybe=true;
    return true;
}

bool byte_counting_bloom_validate(const ByteCountingBloom *filter) {
    if(filter==NULL||filter->counter_count==0||
       filter->hash_count==0||filter->counters==NULL||
       filter->nonzero_count>filter->counter_count) {
        return false;
    }

    size_t nonzero=0;

    for(size_t i=0;i<filter->counter_count;++i) {
        nonzero+=filter->counters[i]!=0U?1U:0U;
    }

    return nonzero==filter->nonzero_count;
}
