#include "u64_ordered.h"
#include <assert.h>
#include <stdio.h>
int main(void){U64OrderedMap*m=u64_ordered_map_create();assert(m);u64_ordered_map_put(m,9,90);u64_ordered_map_put(m,2,20);u64_ordered_map_put(m,5,50);for(size_t i=0;i<u64_ordered_map_size(m);++i){uint64_t k;int64_t v;u64_ordered_map_nth(m,i,&k,&v);printf("%llu ",(unsigned long long)k);}putchar('\n');u64_ordered_map_free(m);return 0;}
