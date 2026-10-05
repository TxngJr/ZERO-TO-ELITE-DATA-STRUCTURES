#include "u64_ordered.h"
#include <assert.h>
#include <stdio.h>
int main(void){U64OrderedMap*m=u64_ordered_map_create();U64OrderedSet*s=u64_ordered_set_create();assert(m&&s);for(uint64_t i=1000;i-->0;){assert(u64_ordered_map_put(m,i,(int64_t)i*3));assert(u64_ordered_set_add(s,i));}for(size_t i=0;i<1000;++i){uint64_t k=0,sk=0;int64_t v=0;assert(u64_ordered_map_nth(m,i,&k,&v)&&k==i&&v==(int64_t)i*3);assert(u64_ordered_set_nth(s,i,&sk)&&sk==i);}assert(u64_ordered_map_put(m,500,999));assert(u64_ordered_map_size(m)==1000);for(uint64_t i=0;i<1000;i+=3){assert(u64_ordered_map_remove(m,i));assert(u64_ordered_set_remove(s,i));}assert(u64_ordered_map_validate(m));assert(u64_ordered_set_validate(s));u64_ordered_set_free(s);u64_ordered_map_free(m);puts("Ordered Map/Set tests passed");return 0;}
