#include "free_list_allocator.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdalign.h>

typedef struct Extent {
    size_t offset;
    size_t size;
    size_t requested;
    struct Extent *next;
} Extent;

struct FreeListAllocator {
    unsigned char *storage;
    size_t capacity;
    size_t allocated_bytes;
    size_t allocation_count;
    Extent *free_list;
    Extent *allocations;
};

static bool is_pow2(size_t x){return x!=0U && (x&(x-1U))==0U;}

static bool aligned_offset(const FreeListAllocator*a,size_t offset,size_t alignment,size_t*out){
    uintptr_t base=(uintptr_t)a->storage;
    if(base>UINTPTR_MAX-offset)return false;
    uintptr_t p=base+offset;
    uintptr_t mask=(uintptr_t)(alignment-1U);
    if(p>UINTPTR_MAX-mask)return false;
    uintptr_t q=(p+mask)&~mask;
    if(q<base)return false;
    uintptr_t delta=q-base;
    if(delta>SIZE_MAX)return false;
    *out=(size_t)delta;
    return true;
}

FreeListAllocator*fla_create(size_t capacity){
    if(capacity==0U)return NULL;
    size_t alignment=alignof(max_align_t);
    if(capacity>SIZE_MAX-(alignment-1U))return NULL;
    size_t storage_size=((capacity+alignment-1U)/alignment)*alignment;
    FreeListAllocator*a=calloc(1,sizeof(*a));
    if(!a)return NULL;
    a->storage=aligned_alloc(alignment,storage_size);
    if(!a->storage){free(a);return NULL;}
    Extent*n=malloc(sizeof(*n));
    if(!n){free(a->storage);free(a);return NULL;}
    *n=(Extent){0U,capacity,0U,NULL};
    a->capacity=capacity;
    a->free_list=n;
    return a;
}

void fla_free(FreeListAllocator*a){
    if(!a)return;
    Extent*n=a->free_list;
    while(n){Extent*x=n->next;free(n);n=x;}
    n=a->allocations;
    while(n){Extent*x=n->next;free(n);n=x;}
    free(a->storage);
    free(a);
}

void*fla_alloc(FreeListAllocator*a,size_t size,size_t alignment){
    if(!a||size==0U||!is_pow2(alignment)||alignment>4096U)return NULL;
    Extent**link=&a->free_list;
    while(*link){
        Extent*block=*link;
        size_t aligned=0U;
        if(!aligned_offset(a,block->offset,alignment,&aligned)){link=&block->next;continue;}
        if(aligned<block->offset){link=&block->next;continue;}
        size_t prefix=aligned-block->offset;
        if(prefix>block->size||size>block->size-prefix){link=&block->next;continue;}
        size_t suffix=block->size-prefix-size;
        if(a->allocated_bytes>SIZE_MAX-size||a->allocation_count==SIZE_MAX)return NULL;

        Extent*alloc=malloc(sizeof(*alloc));
        if(!alloc)return NULL;
        Extent*suffix_node=NULL;
        if(prefix>0U&&suffix>0U){
            suffix_node=malloc(sizeof(*suffix_node));
            if(!suffix_node){free(alloc);return NULL;}
        }

        *alloc=(Extent){aligned,size,size,a->allocations};
        if(prefix==0U&&suffix==0U){
            *link=block->next;
            free(block);
        }else if(prefix==0U){
            block->offset=aligned+size;
            block->size=suffix;
        }else if(suffix==0U){
            block->size=prefix;
        }else{
            block->size=prefix;
            *suffix_node=(Extent){aligned+size,suffix,0U,block->next};
            block->next=suffix_node;
        }

        a->allocations=alloc;
        a->allocated_bytes+=size;
        ++a->allocation_count;
        return a->storage+aligned;
    }
    return NULL;
}

static bool pointer_offset(const FreeListAllocator*a,const void*ptr,size_t*out){
    if(!a||!ptr)return false;
    uintptr_t base=(uintptr_t)a->storage;
    uintptr_t p=(uintptr_t)ptr;
    if(p<base)return false;
    uintptr_t d=p-base;
    if(d>=a->capacity)return false;
    *out=(size_t)d;
    return true;
}

bool fla_release(FreeListAllocator*a,void*ptr){
    size_t off=0U;
    if(!pointer_offset(a,ptr,&off))return false;
    Extent**alink=&a->allocations;
    while(*alink&&(*alink)->offset!=off)alink=&(*alink)->next;
    if(!*alink)return false;

    Extent*node=*alink;
    *alink=node->next;
    a->allocated_bytes-=node->size;
    --a->allocation_count;
    node->requested=0U;

    Extent**flink=&a->free_list;
    Extent*prev=NULL;
    while(*flink&&(*flink)->offset<node->offset){
        prev=*flink;
        flink=&(*flink)->next;
    }
    node->next=*flink;
    *flink=node;

    if(node->next&&node->offset+node->size==node->next->offset){
        Extent*next=node->next;
        node->size+=next->size;
        node->next=next->next;
        free(next);
    }
    if(prev&&prev->offset+prev->size==node->offset){
        prev->size+=node->size;
        prev->next=node->next;
        free(node);
    }
    return true;
}

size_t fla_capacity(const FreeListAllocator*a){return a?a->capacity:0U;}
size_t fla_allocated_bytes(const FreeListAllocator*a){return a?a->allocated_bytes:0U;}
size_t fla_allocation_count(const FreeListAllocator*a){return a?a->allocation_count:0U;}
size_t fla_free_bytes(const FreeListAllocator*a){return a?a->capacity-a->allocated_bytes:0U;}

size_t fla_free_block_count(const FreeListAllocator*a){
    size_t count=0U;
    if(a)for(Extent*n=a->free_list;n;n=n->next)++count;
    return count;
}

size_t fla_largest_free_block(const FreeListAllocator*a){
    size_t largest=0U;
    if(a)for(Extent*n=a->free_list;n;n=n->next)if(n->size>largest)largest=n->size;
    return largest;
}

bool fla_validate(const FreeListAllocator*a){
    if(!a||!a->storage||a->capacity==0U)return false;
    size_t free_sum=0U,alloc_sum=0U,alloc_count=0U;
    size_t last_end=0U;
    bool first=true;

    for(Extent*n=a->free_list;n;n=n->next){
        if(n->size==0U||n->offset>a->capacity||n->size>a->capacity-n->offset)return false;
        if(!first&&n->offset<=last_end)return false;
        first=false;
        last_end=n->offset+n->size;
        if(free_sum>SIZE_MAX-n->size)return false;
        free_sum+=n->size;
    }

    for(Extent*n=a->allocations;n;n=n->next){
        if(n->size==0U||n->requested!=n->size||n->offset>a->capacity||n->size>a->capacity-n->offset)return false;
        if(alloc_sum>SIZE_MAX-n->size||alloc_count==SIZE_MAX)return false;
        alloc_sum+=n->size;
        ++alloc_count;

        for(Extent*m=n->next;m;m=m->next){
            size_t a0=n->offset,a1=n->offset+n->size;
            size_t b0=m->offset,b1=m->offset+m->size;
            if(a0<b1&&b0<a1)return false;
        }
        for(Extent*f=a->free_list;f;f=f->next){
            size_t a0=n->offset,a1=n->offset+n->size;
            size_t b0=f->offset,b1=f->offset+f->size;
            if(a0<b1&&b0<a1)return false;
        }
    }

    return alloc_sum==a->allocated_bytes &&
           alloc_count==a->allocation_count &&
           free_sum+alloc_sum==a->capacity;
}
