#include "memory_layout.h"
#include <stdint.h>
#include <stdlib.h>

struct MaAlignedArray {
    unsigned char *raw;
    unsigned char *base;
    size_t allocation_bytes;
    size_t count;
    size_t element_size;
    size_t alignment;
    size_t stride;
};

static bool power_of_two(size_t x){
    return x!=0U&&(x&(x-1U))==0U;
}

bool ma_align_up(size_t value,size_t alignment,size_t*out){
    if(!out||!power_of_two(alignment))return false;
    size_t mask=alignment-1U;
    if(value>SIZE_MAX-mask)return false;
    *out=(value+mask)&~mask;
    return true;
}

bool ma_compute_layout(const MaField*fields,size_t count,MaLayout*out){
    if(!fields||!out||count==0U||count>32U)return false;
    size_t offset=0U,max_alignment=1U,sum=0U;
    MaLayout result={0};
    result.field_count=count;

    for(size_t i=0;i<count;++i){
        size_t size=fields[i].size,alignment=fields[i].alignment;
        if(size==0U||!power_of_two(alignment))return false;
        size_t aligned=0U;
        if(!ma_align_up(offset,alignment,&aligned)||
           size>SIZE_MAX-aligned||
           size>SIZE_MAX-sum)return false;
        result.offsets[i]=aligned;
        offset=aligned+size;
        sum+=size;
        if(alignment>max_alignment)max_alignment=alignment;
    }

    size_t struct_size=0U;
    if(!ma_align_up(offset,max_alignment,&struct_size)||struct_size<sum)
        return false;
    result.struct_size=struct_size;
    result.struct_alignment=max_alignment;
    result.padding_bytes=struct_size-sum;
    *out=result;
    return true;
}

MaAlignedArray *ma_aligned_array_create(size_t count,size_t element_size,
                                        size_t alignment){
    if(count==0U||element_size==0U||!power_of_two(alignment))return NULL;
    size_t stride=0U;
    if(!ma_align_up(element_size,alignment,&stride)||
       count>SIZE_MAX/stride)return NULL;
    size_t logical=count*stride;
    size_t slack=alignment-1U;
    if(logical>SIZE_MAX-slack)return NULL;
    size_t allocation=logical+slack;

    MaAlignedArray*a=calloc(1,sizeof(*a));
    if(!a)return NULL;
    a->raw=malloc(allocation);
    if(!a->raw){free(a);return NULL;}

    uintptr_t address=(uintptr_t)a->raw;
    uintptr_t mask=(uintptr_t)alignment-1U;
    if(address>UINTPTR_MAX-mask){
        free(a->raw);free(a);return NULL;
    }
    uintptr_t aligned=(address+mask)&~mask;
    a->base=(unsigned char*)aligned;
    a->allocation_bytes=allocation;
    a->count=count;a->element_size=element_size;
    a->alignment=alignment;a->stride=stride;
    return a;
}

void ma_aligned_array_free(MaAlignedArray*a){
    if(!a)return;
    free(a->raw);
    free(a);
}

void *ma_aligned_array_get(MaAlignedArray*a,size_t index){
    if(!a||index>=a->count)return NULL;
    return a->base+index*a->stride;
}

const void *ma_aligned_array_get_const(const MaAlignedArray*a,size_t index){
    if(!a||index>=a->count)return NULL;
    return a->base+index*a->stride;
}

size_t ma_aligned_array_count(const MaAlignedArray*a){return a?a->count:0U;}
size_t ma_aligned_array_element_size(const MaAlignedArray*a){return a?a->element_size:0U;}
size_t ma_aligned_array_alignment(const MaAlignedArray*a){return a?a->alignment:0U;}
size_t ma_aligned_array_stride(const MaAlignedArray*a){return a?a->stride:0U;}
size_t ma_aligned_array_storage_bytes(const MaAlignedArray*a){
    return a?a->count*a->stride:0U;
}

bool ma_aligned_array_validate(const MaAlignedArray*a){
    if(!a||!a->raw||!a->base||a->count==0U||a->element_size==0U||
       !power_of_two(a->alignment)||a->stride<a->element_size||
       a->stride%a->alignment!=0U||
       (uintptr_t)a->base%(uintptr_t)a->alignment!=0U)return false;

    uintptr_t raw=(uintptr_t)a->raw,base=(uintptr_t)a->base;
    if(base<raw||base-raw>=a->alignment)return false;
    if(a->count>SIZE_MAX/a->stride)return false;
    size_t logical=a->count*a->stride;
    if(logical>a->allocation_bytes-(size_t)(base-raw))return false;

    for(size_t i=0;i<a->count;++i){
        const void*p=a->base+i*a->stride;
        if((uintptr_t)p%(uintptr_t)a->alignment!=0U)return false;
    }
    return true;
}
