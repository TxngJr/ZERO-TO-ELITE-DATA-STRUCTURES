#include "u64_cuckoo_set.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t rng_next(uint64_t *s){*s=*s*UINT64_C(6364136223846793005)+UINT64_C(1442695040888963407);return *s;}

int main(void){
    U64CuckooSet *set=u64_cuckoo_set_create(8);assert(set);
    enum { N=20000 };
    for(uint64_t i=0;i<N;++i)assert(u64_cuckoo_set_insert(set,i*17+3));
    assert(u64_cuckoo_set_size(set)==N);
    for(uint64_t i=0;i<N;++i)assert(u64_cuckoo_set_contains(set,i*17+3));
    for(uint64_t i=0;i<N;i+=3)assert(u64_cuckoo_set_remove(set,i*17+3));
    assert(u64_cuckoo_set_validate(set));

    uint64_t rng=UINT64_C(0x71ABCDEF12345678);
    for(int i=0;i<30000;++i){
        uint64_t k=rng_next(&rng)%50000U;
        if((rng_next(&rng)&1U)!=0)assert(u64_cuckoo_set_insert(set,k));
        else (void)u64_cuckoo_set_remove(set,k);
        if((i%997)==0)assert(u64_cuckoo_set_validate(set));
    }
    assert(u64_cuckoo_set_validate(set));
    u64_cuckoo_set_free(set);
    puts("Cuckoo Hashing tests passed");
    return 0;
}
