#include "u64_reservoir.h"
#include <assert.h>
#include <stdio.h>
int main(void){U64Reservoir *r=u64_reservoir_create(5,42);assert(r);for(uint64_t i=0;i<100;++i)assert(u64_reservoir_add(r,i));printf("seen=%llu sample:",(unsigned long long)u64_reservoir_seen(r));for(size_t i=0;i<5;++i){uint64_t v=0;assert(u64_reservoir_get(r,i,&v));printf(" %llu",(unsigned long long)v);}putchar('\n');u64_reservoir_free(r);return 0;}
