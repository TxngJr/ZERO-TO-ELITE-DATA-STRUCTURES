#include "u64_perfect_set.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum { TOP_TRIES=2048, SECONDARY_TRIES=4096 };

typedef struct {
    size_t slot_count;
    size_t key_count;
    uint64_t seed;
    uint64_t *keys;
    uint8_t *used;
} PerfectBucket;

struct U64PerfectSet {
    size_t size;
    size_t bucket_count;
    size_t secondary_slots;
    uint64_t top_seed;
    PerfectBucket *buckets;
};

static uint64_t mix64(uint64_t x){
    x^=x>>30;x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27;x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31;return x;
}

static int cmp_u64(const void *a,const void *b){
    uint64_t x=*(const uint64_t*)a,y=*(const uint64_t*)b;
    return x<y?-1:(x>y?1:0);
}

static size_t top_index(uint64_t key,uint64_t seed,size_t m){return m==0?0:(size_t)(mix64(key^seed)%(uint64_t)m);}
static size_t second_index(uint64_t key,uint64_t seed,size_t m){return (size_t)(mix64(key^seed)%(uint64_t)m);}

static void free_bucket(PerfectBucket *b){free(b->keys);free(b->used);b->keys=NULL;b->used=NULL;}

void u64_perfect_set_free(U64PerfectSet *s){
    if(!s)return;
    if(s->buckets)for(size_t i=0;i<s->bucket_count;++i)free_bucket(&s->buckets[i]);
    free(s->buckets);free(s);
}

static bool choose_top_seed(const uint64_t *keys,size_t n,size_t m,uint64_t *out_seed,size_t *counts){
    for(uint64_t attempt=1;attempt<=TOP_TRIES;++attempt){
        memset(counts,0,m*sizeof *counts);
        const uint64_t seed=mix64(UINT64_C(0x9e3779b97f4a7c15)*attempt);
        for(size_t i=0;i<n;++i)++counts[top_index(keys[i],seed,m)];
        size_t sumsq=0;bool overflow=false;
        for(size_t b=0;b<m;++b){
            if(counts[b]!=0&&counts[b]>SIZE_MAX/counts[b]){overflow=true;break;}
            const size_t sq=counts[b]*counts[b];
            if(SIZE_MAX-sumsq<sq){overflow=true;break;}
            sumsq+=sq;
        }
        const size_t bound=n>SIZE_MAX/4?SIZE_MAX:4*n;
        if(!overflow&&sumsq<=bound){*out_seed=seed;return true;}
    }
    return false;
}

U64PerfectSet *u64_perfect_set_create(const uint64_t *input,size_t count){
    if(count>0&&input==NULL)return NULL;
    U64PerfectSet *s=calloc(1,sizeof *s);if(!s)return NULL;
    if(count==0)return s;
    if(count>SIZE_MAX/sizeof(uint64_t)){free(s);return NULL;}

    uint64_t *keys=malloc(count*sizeof *keys);if(!keys){free(s);return NULL;}
    memcpy(keys,input,count*sizeof *keys);qsort(keys,count,sizeof *keys,cmp_u64);
    size_t n=0;for(size_t i=0;i<count;++i)if(i==0||keys[i]!=keys[i-1])keys[n++]=keys[i];
    s->size=n;s->bucket_count=n;

    size_t *counts=calloc(n,sizeof *counts);
    if(!counts||!choose_top_seed(keys,n,n,&s->top_seed,counts)){free(counts);free(keys);free(s);return NULL;}

    s->buckets=calloc(n,sizeof *s->buckets);
    if(!s->buckets){free(counts);free(keys);free(s);return NULL;}

    uint64_t **groups=calloc(n,sizeof *groups);
    size_t *fill=calloc(n,sizeof *fill);
    if(!groups||!fill){free(groups);free(fill);free(counts);free(keys);u64_perfect_set_free(s);return NULL;}

    for(size_t b=0;b<n;++b){
        if(counts[b]>0){groups[b]=malloc(counts[b]*sizeof(uint64_t));if(!groups[b]){
            for(size_t j=0;j<b;++j)free(groups[j]);free(groups);free(fill);free(counts);free(keys);u64_perfect_set_free(s);return NULL;
        }}
    }
    for(size_t i=0;i<n;++i){size_t b=top_index(keys[i],s->top_seed,n);groups[b][fill[b]++]=keys[i];}

    bool ok=true;
    for(size_t b=0;b<n&&ok;++b){
        PerfectBucket *pb=&s->buckets[b];pb->key_count=counts[b];
        if(counts[b]==0)continue;
        if(counts[b]>SIZE_MAX/counts[b]){ok=false;break;}
        pb->slot_count=counts[b]*counts[b];
        if(SIZE_MAX-s->secondary_slots<pb->slot_count){ok=false;break;}
        s->secondary_slots+=pb->slot_count;
        pb->keys=calloc(pb->slot_count,sizeof *pb->keys);
        pb->used=calloc(pb->slot_count,1);
        if(!pb->keys||!pb->used){ok=false;break;}

        bool placed=false;
        for(uint64_t attempt=1;attempt<=SECONDARY_TRIES&&!placed;++attempt){
            memset(pb->used,0,pb->slot_count);
            const uint64_t seed=mix64((uint64_t)b^UINT64_C(0xbf58476d1ce4e5b9)*attempt);
            placed=true;
            for(size_t i=0;i<counts[b];++i){
                const size_t slot=second_index(groups[b][i],seed,pb->slot_count);
                if(pb->used[slot]){placed=false;break;}
                pb->used[slot]=1;pb->keys[slot]=groups[b][i];
            }
            if(placed)pb->seed=seed;
        }
        if(!placed)ok=false;
    }

    for(size_t b=0;b<n;++b)free(groups[b]);
    free(groups);free(fill);free(counts);free(keys);
    if(!ok){u64_perfect_set_free(s);return NULL;}
    return s;
}

size_t u64_perfect_set_size(const U64PerfectSet *s){return s?s->size:0;}
size_t u64_perfect_set_bucket_count(const U64PerfectSet *s){return s?s->bucket_count:0;}
size_t u64_perfect_set_secondary_slots(const U64PerfectSet *s){return s?s->secondary_slots:0;}

bool u64_perfect_set_contains(const U64PerfectSet *s,uint64_t key){
    if(!s||s->size==0)return false;
    const size_t b=top_index(key,s->top_seed,s->bucket_count);
    const PerfectBucket *pb=&s->buckets[b];
    if(pb->slot_count==0)return false;
    const size_t slot=second_index(key,pb->seed,pb->slot_count);
    return pb->used[slot]&&pb->keys[slot]==key;
}

bool u64_perfect_set_validate(const U64PerfectSet *s){
    if(!s)return false;
    if(s->size==0)return s->bucket_count==0&&s->secondary_slots==0&&s->buckets==NULL;
    if(!s->buckets||s->bucket_count!=s->size)return false;
    size_t keys=0,slots=0;
    for(size_t b=0;b<s->bucket_count;++b){
        const PerfectBucket *pb=&s->buckets[b];
        if(pb->key_count==0){if(pb->slot_count!=0||pb->keys||pb->used)return false;continue;}
        if(pb->slot_count!=pb->key_count*pb->key_count||!pb->keys||!pb->used)return false;
        slots+=pb->slot_count;
        size_t local=0;
        for(size_t i=0;i<pb->slot_count;++i)if(pb->used[i]){
            ++local;
            if(top_index(pb->keys[i],s->top_seed,s->bucket_count)!=b)return false;
            if(second_index(pb->keys[i],pb->seed,pb->slot_count)!=i)return false;
        }
        if(local!=pb->key_count)return false;
        keys+=local;
    }
    const size_t bound=
        s->size>SIZE_MAX/4?SIZE_MAX:4*s->size;

    return keys==s->size&&
           slots==s->secondary_slots&&
           slots<=bound;
}
