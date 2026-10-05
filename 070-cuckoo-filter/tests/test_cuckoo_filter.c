#include "byte_cuckoo_filter.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static void encode(uint64_t x,uint8_t out[8]){for(unsigned i=0;i<8;++i)out[i]=(uint8_t)(x>>(8U*i));}

int main(void) {
    assert(byte_cuckoo_filter_create(3)==NULL);
    ByteCuckooFilter *f=byte_cuckoo_filter_create(4096);
    assert(f!=NULL);
    uint8_t key[8];
    enum { N=8000 };

    for(uint64_t i=0;i<N;++i){encode(i,key);assert(byte_cuckoo_filter_add(f,key,8));}
    assert(byte_cuckoo_filter_validate(f));

    for(uint64_t i=0;i<N;++i){
        bool maybe=false;encode(i,key);
        assert(byte_cuckoo_filter_maybe_contains(f,key,8,&maybe));
        assert(maybe);
    }

    for(uint64_t i=0;i<N;i+=2){encode(i,key);assert(byte_cuckoo_filter_remove(f,key,8));}
    for(uint64_t i=1;i<N;i+=2){
        bool maybe=false;encode(i,key);
        assert(byte_cuckoo_filter_maybe_contains(f,key,8,&maybe));
        assert(maybe);
    }

    assert(byte_cuckoo_filter_size(f)==N/2);
    assert(byte_cuckoo_filter_validate(f));
    byte_cuckoo_filter_free(f);
    puts("Cuckoo Filter tests passed");
    return 0;
}
