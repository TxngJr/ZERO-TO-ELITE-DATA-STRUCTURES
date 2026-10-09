#include "cache_sim.h"
#include <assert.h>
#include <stdio.h>
int main(void){
    CacheSim*d=cache_sim_create(64,2,1);assert(d);
    uint64_t direct[]={0,64,128,0};
    assert(cache_sim_access_range(d,direct,4)&&cache_sim_validate(d));
    CacheStats s=cache_sim_stats(d);
    assert(s.accesses==4U&&s.hits==0U&&s.misses==4U&&s.evictions==2U);

    CacheSim*lru=cache_sim_create(64,1,2);assert(lru);
    uint64_t trace[]={0,64,0,128,64};
    assert(cache_sim_access_range(lru,trace,5)&&cache_sim_validate(lru));
    s=cache_sim_stats(lru);
    assert(s.accesses==5U&&s.hits==1U&&s.misses==4U&&s.evictions==2U);
    cache_sim_reset(lru);s=cache_sim_stats(lru);
    assert(s.accesses==0U&&s.hits==0U&&s.misses==0U&&s.evictions==0U&&cache_sim_validate(lru));

    CacheSim*m=cache_sim_create(64,16,2);assert(m);
    enum{R=64,C=64};
    for(size_t r=0;r<R;++r)for(size_t c=0;c<C;++c){
        uint64_t address=(uint64_t)(r*C+c)*4U;
        assert(cache_sim_access(m,address,NULL));
    }
    CacheStats row=cache_sim_stats(m);
    cache_sim_reset(m);
    for(size_t c=0;c<C;++c)for(size_t r=0;r<R;++r){
        uint64_t address=(uint64_t)(r*C+c)*4U;
        assert(cache_sim_access(m,address,NULL));
    }
    CacheStats col=cache_sim_stats(m);
    assert(row.misses==256U);
    assert(col.misses>row.misses);
    assert(cache_sim_validate(m));

    assert(!cache_sim_create(0,1,1));
    assert(!cache_sim_create(64,0,1));
    assert(!cache_sim_create(64,1,0));
    printf("Cache simulator tests passed; row_misses=%llu column_misses=%llu\n",
           (unsigned long long)row.misses,(unsigned long long)col.misses);
    cache_sim_free(m);cache_sim_free(lru);cache_sim_free(d);
    return 0;
}
