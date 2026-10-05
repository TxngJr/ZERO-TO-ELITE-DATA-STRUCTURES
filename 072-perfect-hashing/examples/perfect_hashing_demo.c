#include "u64_perfect_set.h"
#include <assert.h>
#include <stdio.h>
int main(void){uint64_t keys[]={10,20,30,40,20};U64PerfectSet *s=u64_perfect_set_create(keys,5);assert(s);printf("unique=%zu buckets=%zu secondary=%zu\n",u64_perfect_set_size(s),u64_perfect_set_bucket_count(s),u64_perfect_set_secondary_slots(s));assert(u64_perfect_set_contains(s,30));assert(u64_perfect_set_validate(s));u64_perfect_set_free(s);return 0;}
