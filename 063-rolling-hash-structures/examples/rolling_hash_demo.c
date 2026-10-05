#include "byte_rolling_hash.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    const uint8_t text[]={'b','a','n','a','n','a'};

    ByteRollingHash *hash=
        byte_rolling_hash_create(text,sizeof text);

    assert(hash!=NULL);

    bool equal=false;

    assert(byte_rolling_hash_equal(
        hash,1,4,3,6,&equal
    ));

    const uint8_t pattern[]={'a','n','a'};
    size_t count=0;

    assert(byte_rolling_hash_count(
        hash,pattern,sizeof pattern,&count
    ));

    ByteHashPair h;

    assert(byte_rolling_hash_range(hash,1,4,&h));

    printf(
        "ana_equal=%d occurrences=%zu hash=(%" PRIu64 ",%" PRIu64 ")\n",
        equal?1:0,count,h.first,h.second
    );

    assert(byte_rolling_hash_validate(hash));
    byte_rolling_hash_free(hash);
    return 0;
}
