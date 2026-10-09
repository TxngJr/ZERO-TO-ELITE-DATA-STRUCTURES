#include "cache_sim.h"
#include <stdlib.h>
#include <string.h>

struct CacheSim {
    size_t line_size;
    size_t set_count;
    size_t associativity;
    uint64_t *tags;
    size_t *valid_counts;
    CacheStats stats;
};

CacheSim *cache_sim_create(size_t line_size,size_t set_count,size_t associativity){
    if(line_size==0U||set_count==0U||associativity==0U||
       set_count>SIZE_MAX/associativity)return NULL;
    size_t entries=set_count*associativity;
    if(entries>SIZE_MAX/sizeof(uint64_t)||
       set_count>SIZE_MAX/sizeof(size_t))return NULL;

    CacheSim*s=calloc(1,sizeof(*s));
    if(!s)return NULL;
    s->tags=calloc(entries,sizeof(*s->tags));
    s->valid_counts=calloc(set_count,sizeof(*s->valid_counts));
    if(!s->tags||!s->valid_counts){
        cache_sim_free(s);
        return NULL;
    }
    s->line_size=line_size;
    s->set_count=set_count;
    s->associativity=associativity;
    return s;
}

void cache_sim_free(CacheSim*s){
    if(!s)return;
    free(s->tags);
    free(s->valid_counts);
    free(s);
}

static uint64_t *set_tags(CacheSim*s,size_t set){
    return s->tags+set*s->associativity;
}

bool cache_sim_access(CacheSim*s,uint64_t address,bool*out_hit){
    if(!s)return false;
    uint64_t line=address/(uint64_t)s->line_size;
    size_t set=(size_t)(line%(uint64_t)s->set_count);
    uint64_t tag=line/(uint64_t)s->set_count;
    uint64_t *tags=set_tags(s,set);
    size_t valid=s->valid_counts[set];

    ++s->stats.accesses;
    for(size_t i=0;i<valid;++i){
        if(tags[i]==tag){
            uint64_t hit_tag=tags[i];
            if(i)memmove(tags+1U,tags,i*sizeof(*tags));
            tags[0]=hit_tag;
            ++s->stats.hits;
            if(out_hit)*out_hit=true;
            return true;
        }
    }

    ++s->stats.misses;
    if(valid<s->associativity){
        if(valid)memmove(tags+1U,tags,valid*sizeof(*tags));
        tags[0]=tag;
        s->valid_counts[set]=valid+1U;
    }else{
        if(s->associativity>1U)
            memmove(tags+1U,tags,(s->associativity-1U)*sizeof(*tags));
        tags[0]=tag;
        ++s->stats.evictions;
    }
    if(out_hit)*out_hit=false;
    return true;
}

bool cache_sim_access_range(CacheSim*s,const uint64_t*addresses,size_t count){
    if(!s||(count&& !addresses))return false;
    for(size_t i=0;i<count;++i)
        if(!cache_sim_access(s,addresses[i],NULL))return false;
    return true;
}

void cache_sim_reset(CacheSim*s){
    if(!s)return;
    size_t entries=s->set_count*s->associativity;
    memset(s->tags,0,entries*sizeof(*s->tags));
    memset(s->valid_counts,0,s->set_count*sizeof(*s->valid_counts));
    memset(&s->stats,0,sizeof(s->stats));
}

CacheStats cache_sim_stats(const CacheSim*s){
    CacheStats zero={0};
    return s?s->stats:zero;
}

size_t cache_sim_line_size(const CacheSim*s){return s?s->line_size:0U;}
size_t cache_sim_set_count(const CacheSim*s){return s?s->set_count:0U;}
size_t cache_sim_associativity(const CacheSim*s){return s?s->associativity:0U;}

bool cache_sim_validate(const CacheSim*s){
    if(!s||!s->tags||!s->valid_counts||s->line_size==0U||
       s->set_count==0U||s->associativity==0U)return false;
    if(s->stats.hits+s->stats.misses!=s->stats.accesses||
       s->stats.evictions>s->stats.misses)return false;

    for(size_t set=0;set<s->set_count;++set){
        size_t valid=s->valid_counts[set];
        if(valid>s->associativity)return false;
        const uint64_t *tags=s->tags+set*s->associativity;
        for(size_t i=0;i<valid;++i)
            for(size_t j=i+1U;j<valid;++j)
                if(tags[i]==tags[j])return false;
    }
    return true;
}
