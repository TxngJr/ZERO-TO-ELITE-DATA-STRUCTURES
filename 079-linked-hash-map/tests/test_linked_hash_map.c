#include "u64_linked_hash_map.h"
#include <assert.h>
#include <stdio.h>
int main(void){U64LinkedHashMap*m=u64_lhm_create(4);assert(m);for(uint64_t i=0;i<1000;++i)assert(u64_lhm_put(m,i,(int64_t)i*10));assert(u64_lhm_size(m)==1000);assert(u64_lhm_put(m,10,999));uint64_t k=0;int64_t v=0;assert(u64_lhm_nth(m,10,&k,&v)&&k==10&&v==999);for(uint64_t i=0;i<1000;i+=2)assert(u64_lhm_remove(m,i));assert(u64_lhm_validate(m));size_t idx=0;for(uint64_t expected=1;expected<1000;expected+=2){assert(u64_lhm_nth(m,idx++,&k,&v));assert(k==expected);assert(v==(int64_t)expected*10);}u64_lhm_free(m);puts("Linked Hash Map tests passed");return 0;}
