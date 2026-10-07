#include "memory_layout.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
int main(void){
    MaField a[3]={{1,1},{8,8},{4,4}};MaLayout la;
    assert(ma_compute_layout(a,3,&la));
    assert(la.offsets[0]==0U&&la.offsets[1]==8U&&la.offsets[2]==16U);
    assert(la.struct_size==24U&&la.struct_alignment==8U&&la.padding_bytes==11U);

    MaField b[3]={{8,8},{4,4},{1,1}};MaLayout lb;
    assert(ma_compute_layout(b,3,&lb));
    assert(lb.offsets[0]==0U&&lb.offsets[1]==8U&&lb.offsets[2]==12U);
    assert(lb.struct_size==16U&&lb.padding_bytes==3U);

    size_t x=0;assert(ma_align_up(0,8,&x)&&x==0U);
    assert(ma_align_up(9,8,&x)&&x==16U);
    assert(!ma_align_up(7,3,&x));
    assert(!ma_align_up(SIZE_MAX,8,&x));
    MaField bad={1,3};assert(!ma_compute_layout(&bad,1,&la));
    MaField overflow[2]={{SIZE_MAX-7U,1U},{16U,8U}};
    assert(!ma_compute_layout(overflow,2,&la));

    MaAlignedArray*arr=ma_aligned_array_create(10000,24,64);assert(arr);
    assert(ma_aligned_array_stride(arr)==64U&&ma_aligned_array_validate(arr));
    for(size_t i=0;i<10000U;++i){
        unsigned char*p=ma_aligned_array_get(arr,i);assert(p&&((uintptr_t)p%64U)==0U);
        memset(p,(int)(i&255U),24U);
    }
    assert(ma_aligned_array_storage_bytes(arr)==640000U);
    ma_aligned_array_free(arr);

    arr=ma_aligned_array_create(1000,80,64);assert(arr);
    assert(ma_aligned_array_stride(arr)==128U&&ma_aligned_array_validate(arr));
    assert(ma_aligned_array_get(arr,1000)==NULL);
    ma_aligned_array_free(arr);

    puts("Memory alignment/layout tests passed");
    return 0;
}
