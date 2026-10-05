#include "byte_bloom_filter.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    ByteBloomFilter *filter=byte_bloom_filter_create(4096,5);
    assert(filter!=NULL);

    const uint8_t *alice=(const uint8_t *)"alice";
    const uint8_t *bob=(const uint8_t *)"bob";

    assert(byte_bloom_filter_add(filter,alice,5));

    bool maybe=false;
    assert(byte_bloom_filter_maybe_contains(filter,alice,5,&maybe));
    printf("alice maybe=%d\n",maybe?1:0);

    assert(byte_bloom_filter_maybe_contains(filter,bob,3,&maybe));
    printf("bob maybe=%d\n",maybe?1:0);

    assert(byte_bloom_filter_validate(filter));
    byte_bloom_filter_free(filter);
    return 0;
}
