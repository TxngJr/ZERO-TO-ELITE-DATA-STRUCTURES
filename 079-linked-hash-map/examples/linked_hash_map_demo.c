#include "u64_linked_hash_map.h"
#include <assert.h>
#include <stdio.h>
int main(void){U64LinkedHashMap*m=u64_lhm_create(4);assert(m);assert(u64_lhm_put(m,7,70));assert(u64_lhm_put(m,2,20));assert(u64_lhm_put(m,9,90));for(size_t i=0;i<u64_lhm_size(m);++i){uint64_t k;int64_t v;assert(u64_lhm_nth(m,i,&k,&v));printf("%llu:%lld ",(unsigned long long)k,(long long)v);}putchar('\n');u64_lhm_free(m);return 0;}
