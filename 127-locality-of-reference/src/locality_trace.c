#include "locality_trace.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t key;
    unsigned char used;
} LineSlot;

static uint64_t mix64(uint64_t x){
    x^=x>>30U;x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27U;x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31U;
    return x;
}

static bool set_capacity_for(size_t count,size_t *out){
    if(!out)return false;
    size_t need=count<4U?8U:count;
    if(need>SIZE_MAX/2U)return false;
    need*=2U;
    size_t cap=8U;
    while(cap<need){
        if(cap>SIZE_MAX/2U)return false;
        cap*=2U;
    }
    *out=cap;
    return true;
}

static bool line_set_insert(LineSlot *slots,size_t capacity,size_t key,bool *inserted){
    size_t mask=capacity-1U;
    size_t pos=(size_t)mix64((uint64_t)key)&mask;
    for(size_t probes=0;probes<capacity;++probes){
        LineSlot *slot=&slots[pos];
        if(!slot->used){
            slot->used=1U;
            slot->key=key;
            *inserted=true;
            return true;
        }
        if(slot->key==key){
            *inserted=false;
            return true;
        }
        pos=(pos+1U)&mask;
    }
    return false;
}

bool loc_analyze_indices(const size_t *indices,size_t count,size_t element_size,
                         size_t line_size,LocalityStats *out){
    if(!out||element_size==0U||line_size==0U||(count&& !indices))return false;
    LocalityStats stats={0};
    stats.accesses=count;
    if(count==0U){
        *out=stats;
        return true;
    }

    size_t capacity=0U;
    if(!set_capacity_for(count,&capacity)||
       capacity>SIZE_MAX/sizeof(LineSlot))return false;
    LineSlot *slots=calloc(capacity,sizeof(*slots));
    if(!slots)return false;

    size_t previous_line=0U;
    for(size_t i=0;i<count;++i){
        size_t index=indices[i];
        if(index>SIZE_MAX/element_size){
            free(slots);
            return false;
        }
        size_t address=index*element_size;
        size_t line=address/line_size;
        bool inserted=false;
        if(!line_set_insert(slots,capacity,line,&inserted)){
            free(slots);
            return false;
        }
        if(inserted)++stats.unique_lines;
        else ++stats.temporal_line_reuses;

        if(i==0U||index>stats.max_index)stats.max_index=index;
        if(i){
            if(line==previous_line){
                ++stats.same_line_adjacent;
            }else{
                ++stats.line_transitions;
                size_t delta=line>previous_line?line-previous_line:previous_line-line;
                if(delta==1U)++stats.adjacent_line_transitions;
            }
        }
        previous_line=line;
    }
    stats.first_line_touches=stats.unique_lines;
    free(slots);
    *out=stats;
    return true;
}

bool loc_generate_sequential(size_t count,size_t *out){
    if(count&& !out)return false;
    for(size_t i=0;i<count;++i)out[i]=i;
    return true;
}

static size_t gcd_size(size_t a,size_t b){
    while(b){
        size_t r=a%b;
        a=b;
        b=r;
    }
    return a;
}

bool loc_generate_strided_permutation(size_t count,size_t stride,size_t *out){
    if(count==0U)return true;
    if(!out||stride==0U||gcd_size(count,stride)!=1U)return false;
    size_t current=0U;
    for(size_t i=0;i<count;++i){
        out[i]=current;
        if(current>=count-stride%count)
            current=current-(count-stride%count);
        else
            current+=stride%count;
    }
    return true;
}
