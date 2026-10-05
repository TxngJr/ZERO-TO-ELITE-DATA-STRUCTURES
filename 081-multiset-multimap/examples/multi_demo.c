#include "u64_multi.h"
#include <assert.h>
#include <stdio.h>
int main(void){U64Multiset*s=u64_multiset_create();assert(s);u64_multiset_add(s,42,3);printf("count(42)=%zu total=%zu\n",u64_multiset_count(s,42),u64_multiset_size(s));u64_multiset_free(s);return 0;}
