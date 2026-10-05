#include "byte_cuckoo_filter.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum { BUCKET_SIZE=4, MAX_KICKS=500 };

struct ByteCuckooFilter {
    size_t bucket_count;
    size_t size;
    uint16_t *slots;
    uint64_t rng;
};

static uint64_t mix64(uint64_t x) {
    x^=x>>30;
    x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27;
    x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31;
    return x;
}

static uint64_t hash_bytes(const uint8_t *key,size_t length) {
    uint64_t h=UINT64_C(1469598103934665603);
    for(size_t i=0;i<length;++i) {
        h^=key[i];
        h*=UINT64_C(1099511628211);
    }
    return mix64(h^(uint64_t)length);
}

static uint16_t fingerprint(uint64_t h) {
    uint16_t fp=(uint16_t)(mix64(h)>>48);
    return fp==0?1:fp;
}

static size_t alt_index(const ByteCuckooFilter *f,size_t index,uint16_t fp) {
    const size_t mask=f->bucket_count-1;
    const size_t delta=(size_t)mix64((uint64_t)fp)&mask;
    return index^delta;
}

static uint64_t next_rng(ByteCuckooFilter *f) {
    f->rng=f->rng*UINT64_C(6364136223846793005)+UINT64_C(1442695040888963407);
    return f->rng;
}

ByteCuckooFilter *byte_cuckoo_filter_create(size_t bucket_count) {
    if(bucket_count<2||(bucket_count&(bucket_count-1))!=0)return NULL;
    if(bucket_count>SIZE_MAX/BUCKET_SIZE)return NULL;
    const size_t slots=bucket_count*BUCKET_SIZE;
    if(slots>SIZE_MAX/sizeof(uint16_t))return NULL;

    ByteCuckooFilter *f=calloc(1,sizeof *f);
    if(f==NULL)return NULL;
    f->bucket_count=bucket_count;
    f->rng=UINT64_C(0xC0FFEE123456789);
    f->slots=calloc(slots,sizeof *f->slots);
    if(f->slots==NULL){free(f);return NULL;}
    return f;
}

void byte_cuckoo_filter_free(ByteCuckooFilter *f) {
    if(f==NULL)return;
    free(f->slots);
    free(f);
}

size_t byte_cuckoo_filter_size(const ByteCuckooFilter *f){return f==NULL?0:f->size;}
size_t byte_cuckoo_filter_bucket_count(const ByteCuckooFilter *f){return f==NULL?0:f->bucket_count;}

static bool bucket_has(const ByteCuckooFilter *f,size_t b,uint16_t fp) {
    const size_t base=b*BUCKET_SIZE;
    for(size_t i=0;i<BUCKET_SIZE;++i)if(f->slots[base+i]==fp)return true;
    return false;
}

static bool bucket_insert(ByteCuckooFilter *f,size_t b,uint16_t fp) {
    const size_t base=b*BUCKET_SIZE;
    for(size_t i=0;i<BUCKET_SIZE;++i) {
        if(f->slots[base+i]==0){f->slots[base+i]=fp;return true;}
    }
    return false;
}

bool byte_cuckoo_filter_add(ByteCuckooFilter *f,const uint8_t *key,size_t length) {
    if(f==NULL||(length>0&&key==NULL)||f->size==SIZE_MAX)return false;
    const uint64_t h=hash_bytes(key,length);
    const uint16_t fp=fingerprint(h);
    const size_t mask=f->bucket_count-1;
    const size_t i1=(size_t)h&mask;
    const size_t i2=alt_index(f,i1,fp);

    if(bucket_insert(f,i1,fp)||bucket_insert(f,i2,fp)){++f->size;return true;}

    const size_t slot_count=f->bucket_count*BUCKET_SIZE;
    uint16_t *snapshot=malloc(slot_count*sizeof *snapshot);
    if(snapshot==NULL)return false;
    memcpy(snapshot,f->slots,slot_count*sizeof *snapshot);
    const uint64_t old_rng=f->rng;

    size_t index=(next_rng(f)&1U)?i1:i2;
    uint16_t current=fp;

    for(size_t kick=0;kick<MAX_KICKS;++kick) {
        const size_t slot=(size_t)(next_rng(f)%BUCKET_SIZE);
        const size_t pos=index*BUCKET_SIZE+slot;
        const uint16_t evicted=f->slots[pos];
        f->slots[pos]=current;
        current=evicted;
        index=alt_index(f,index,current);

        if(bucket_insert(f,index,current)) {
            ++f->size;
            free(snapshot);
            return true;
        }
    }

    memcpy(f->slots,snapshot,slot_count*sizeof *snapshot);
    f->rng=old_rng;
    free(snapshot);
    return false;
}

bool byte_cuckoo_filter_maybe_contains(const ByteCuckooFilter *f,const uint8_t *key,size_t length,bool *out_maybe) {
    if(f==NULL||out_maybe==NULL||(length>0&&key==NULL))return false;
    const uint64_t h=hash_bytes(key,length);
    const uint16_t fp=fingerprint(h);
    const size_t i1=(size_t)h&(f->bucket_count-1);
    const size_t i2=alt_index(f,i1,fp);
    *out_maybe=bucket_has(f,i1,fp)||bucket_has(f,i2,fp);
    return true;
}

bool byte_cuckoo_filter_remove(ByteCuckooFilter *f,const uint8_t *key,size_t length) {
    if(f==NULL||(length>0&&key==NULL)||f->size==0)return false;
    const uint64_t h=hash_bytes(key,length);
    const uint16_t fp=fingerprint(h);
    const size_t i1=(size_t)h&(f->bucket_count-1);
    const size_t i2=alt_index(f,i1,fp);
    const size_t buckets[2]={i1,i2};

    for(size_t bi=0;bi<2;++bi) {
        const size_t base=buckets[bi]*BUCKET_SIZE;
        for(size_t s=0;s<BUCKET_SIZE;++s) {
            if(f->slots[base+s]==fp) {
                f->slots[base+s]=0;
                --f->size;
                return true;
            }
        }
    }
    return false;
}

bool byte_cuckoo_filter_validate(const ByteCuckooFilter *f) {
    if(f==NULL||f->slots==NULL||f->bucket_count<2||(f->bucket_count&(f->bucket_count-1))!=0)return false;
    size_t occupied=0;
    const size_t total=f->bucket_count*BUCKET_SIZE;
    for(size_t i=0;i<total;++i)occupied+=f->slots[i]!=0?1U:0U;
    return occupied==f->size&&f->size<=total;
}
