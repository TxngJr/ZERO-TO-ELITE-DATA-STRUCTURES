#include "free_list_allocator.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum{CAPACITY=1<<20,SLOTS=2048,STEPS=50000};

typedef struct{void*ptr;size_t size;}Record;
static uint64_t state=UINT64_C(0x1234abcd9876);
static uint64_t rng(void){state^=state<<13;state^=state>>7;state^=state<<17;return state;}

int main(void){
    FreeListAllocator*a=fla_create(CAPACITY);
    assert(a&&fla_validate(a));
    Record records[SLOTS]={0};
    size_t live=0U,bytes=0U;

    for(size_t step=0;step<STEPS;++step){
        bool do_alloc=live==0U||(live<SLOTS&&(rng()%100U)<62U);
        if(do_alloc){
            size_t pos=0U;
            while(pos<SLOTS&&records[pos].ptr)++pos;
            if(pos==SLOTS)continue;
            size_t size=(size_t)(rng()%1024U)+1U;
            size_t alignment=(size_t)1U<<(rng()%7U);
            void*ptr=fla_alloc(a,size,alignment);
            if(!ptr)continue;
            assert(((uintptr_t)ptr&(alignment-1U))==0U);
            memset(ptr,(int)(step&255U),size);
            records[pos]=(Record){ptr,size};
            ++live;
            bytes+=size;
        }else{
            size_t kth=(size_t)(rng()%live),pos=0U;
            for(;;++pos)if(records[pos].ptr){
                if(kth==0U)break;
                --kth;
            }
            void*ptr=records[pos].ptr;
            size_t size=records[pos].size;
            assert(fla_release(a,ptr));
            assert(!fla_release(a,ptr));
            records[pos]=(Record){0};
            --live;
            bytes-=size;
        }
        assert(fla_allocation_count(a)==live);
        assert(fla_allocated_bytes(a)==bytes);
        if(step%499U==0U)assert(fla_validate(a));
    }

    for(size_t i=0;i<SLOTS;++i)
        if(records[i].ptr)assert(fla_release(a,records[i].ptr));

    int foreign=0;
    assert(!fla_release(a,&foreign));
    assert(fla_validate(a));
    assert(fla_free_bytes(a)==CAPACITY);
    assert(fla_free_block_count(a)==1U);
    assert(fla_largest_free_block(a)==CAPACITY);

    puts("Free-list tests passed");
    fla_free(a);
    return 0;
}
