#include "u64_perfect_set.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void){
    U64PerfectSet *empty=u64_perfect_set_create(NULL,0);assert(empty);assert(u64_perfect_set_validate(empty));u64_perfect_set_free(empty);
    enum { N=12000 };
    uint64_t *keys=malloc((N+100)*sizeof *keys);assert(keys);
    for(size_t i=0;i<N;++i)keys[i]=(uint64_t)i*UINT64_C(11400714819323198485)+7;
    for(size_t i=0;i<100;++i)keys[N+i]=keys[i];
    U64PerfectSet *set=u64_perfect_set_create(keys,N+100);assert(set);
    assert(u64_perfect_set_size(set)==N);
    assert(u64_perfect_set_validate(set));
    for(size_t i=0;i<N;++i)assert(u64_perfect_set_contains(set,keys[i]));
    for(uint64_t i=0;i<10000;++i){uint64_t x=UINT64_C(0xdeadbeef00000000)+i;bool naive=false;for(size_t j=0;j<N;++j)if(keys[j]==x){naive=true;break;}assert(u64_perfect_set_contains(set,x)==naive);}
    assert(u64_perfect_set_secondary_slots(set)<=4*u64_perfect_set_size(set));
    u64_perfect_set_free(set);free(keys);
    puts("Perfect Hashing tests passed");
    return 0;
}
