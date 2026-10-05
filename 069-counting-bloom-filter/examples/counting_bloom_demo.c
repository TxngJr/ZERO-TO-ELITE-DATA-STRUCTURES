#include "byte_counting_bloom.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    const uint8_t key[]={42,7,9};

    ByteCountingBloom *filter=byte_counting_bloom_create(4096,5);
    assert(filter!=NULL);

    assert(byte_counting_bloom_add(filter,key,sizeof key));

    bool maybe=false;
    assert(byte_counting_bloom_maybe_contains(
        filter,key,sizeof key,&maybe
    ));

    printf("after add maybe=%d nonzero=%zu\n",
           maybe?1:0,byte_counting_bloom_nonzero_counters(filter));

    assert(byte_counting_bloom_remove(filter,key,sizeof key));

    assert(byte_counting_bloom_maybe_contains(
        filter,key,sizeof key,&maybe
    ));
    printf("after remove maybe=%d\n",maybe?1:0);

    assert(byte_counting_bloom_validate(filter));
    byte_counting_bloom_free(filter);
    return 0;
}
