#include "u64_cuckoo_set.h"
#include <assert.h>
#include <stdio.h>
int main(void){U64CuckooSet *s=u64_cuckoo_set_create(8);assert(s);for(uint64_t i=0;i<100;++i)assert(u64_cuckoo_set_insert(s,i));printf("size=%zu capacity/table=%zu\n",u64_cuckoo_set_size(s),u64_cuckoo_set_table_capacity(s));assert(u64_cuckoo_set_validate(s));u64_cuckoo_set_free(s);return 0;}
