#include "external_memory_set.h"
#include <stdlib.h>

typedef struct{
    size_t start,count;
    uint64_t first,last;
}BlockMeta;

struct ExternalMemorySet{
    uint64_t *values;
    BlockMeta *blocks;
    size_t count,block_capacity,block_count;
    size_t logical_reads,logical_writes;
};

ExternalMemorySet*ems_build(const uint64_t*values,size_t count,size_t cap){
    if(cap==0U||(count>0U&&!values))return NULL;
    for(size_t i=1;i<count;++i)if(values[i-1U]>=values[i])return NULL;

    size_t blocks=count/cap+(count%cap!=0U);
    if(count>SIZE_MAX/sizeof(uint64_t)||blocks>SIZE_MAX/sizeof(BlockMeta))return NULL;

    ExternalMemorySet*s=calloc(1,sizeof(*s));
    if(!s)return NULL;
    if(count){
        s->values=malloc(count*sizeof(*s->values));
        if(!s->values){free(s);return NULL;}
        for(size_t i=0;i<count;++i)s->values[i]=values[i];
    }
    if(blocks){
        s->blocks=malloc(blocks*sizeof(*s->blocks));
        if(!s->blocks){free(s->values);free(s);return NULL;}
    }

    s->count=count;s->block_capacity=cap;s->block_count=blocks;
    for(size_t b=0;b<blocks;++b){
        size_t start=b*cap;
        size_t remaining=count-start;
        size_t n=remaining<cap?remaining:cap;
        s->blocks[b]=(BlockMeta){start,n,s->values[start],s->values[start+n-1U]};
        ++s->logical_writes;
    }
    return s;
}

void ems_free(ExternalMemorySet*s){if(!s)return;free(s->values);free(s->blocks);free(s);}
size_t ems_count(const ExternalMemorySet*s){return s?s->count:0U;}
size_t ems_block_capacity(const ExternalMemorySet*s){return s?s->block_capacity:0U;}
size_t ems_block_count(const ExternalMemorySet*s){return s?s->block_count:0U;}
size_t ems_logical_reads(const ExternalMemorySet*s){return s?s->logical_reads:0U;}
size_t ems_logical_writes(const ExternalMemorySet*s){return s?s->logical_writes:0U;}
void ems_reset_io_counters(ExternalMemorySet*s){if(s){s->logical_reads=0U;s->logical_writes=0U;}}

static size_t first_block_last_ge(const ExternalMemorySet*s,uint64_t key){
    size_t lo=0U,hi=s->block_count;
    while(lo<hi){
        size_t mid=lo+(hi-lo)/2U;
        if(s->blocks[mid].last<key)lo=mid+1U;else hi=mid;
    }
    return lo;
}

bool ems_contains(ExternalMemorySet*s,uint64_t key,bool*out){
    if(!s||!out)return false;
    size_t b=first_block_last_ge(s,key);
    if(b==s->block_count){*out=false;return true;}
    ++s->logical_reads;
    BlockMeta m=s->blocks[b];
    if(key<m.first){*out=false;return true;}

    size_t lo=m.start,hi=m.start+m.count;
    while(lo<hi){
        size_t mid=lo+(hi-lo)/2U;
        if(s->values[mid]<key)lo=mid+1U;else hi=mid;
    }
    *out=lo<m.start+m.count&&s->values[lo]==key;
    return true;
}

bool ems_range_collect(ExternalMemorySet*s,uint64_t low,uint64_t high,
                       uint64_t*out,size_t out_cap,size_t*out_count){
    if(!s||!out_count||low>high)return false;
    size_t produced=0U;
    size_t b=first_block_last_ge(s,low);
    for(;b<s->block_count;++b){
        BlockMeta m=s->blocks[b];
        if(m.first>high)break;
        ++s->logical_reads;
        for(size_t i=m.start;i<m.start+m.count;++i){
            uint64_t v=s->values[i];
            if(v<low)continue;
            if(v>high)break;
            if(produced>=out_cap)return false;
            if(out)out[produced]=v;
            ++produced;
        }
    }
    *out_count=produced;
    return true;
}

bool ems_validate(const ExternalMemorySet*s){
    if(!s||s->block_capacity==0U)return false;
    size_t expected=s->count/s->block_capacity+(s->count%s->block_capacity!=0U);
    if(expected!=s->block_count)return false;
    if((s->count==0U)!=(s->values==NULL))return false;
    if((s->block_count==0U)!=(s->blocks==NULL))return false;
    for(size_t i=1;i<s->count;++i)if(s->values[i-1U]>=s->values[i])return false;

    for(size_t b=0;b<s->block_count;++b){
        BlockMeta m=s->blocks[b];
        size_t start=b*s->block_capacity;
        if(m.start!=start||m.count==0U||m.count>s->block_capacity||
           m.start>s->count||m.count>s->count-m.start)return false;
        if(m.first!=s->values[m.start]||m.last!=s->values[m.start+m.count-1U])return false;
        if(b>0U&&s->blocks[b-1U].last>=m.first)return false;
    }
    return true;
}
