#include "gc_graph.h"
#include <stdio.h>

int main(void){
    GcHeap*h=gc_create(16U);
    if(!h)return 1;
    GcHandle root,a,b;
    if(!gc_alloc(h,1,&root)||!gc_alloc(h,2,&a)||!gc_alloc(h,3,&b))return 2;
    gc_set_edge(h,root,0U,a);
    gc_set_edge(h,a,0U,b);
    gc_add_root(h,root);
    printf("before collect live=%zu\n",gc_live_count(h));
    printf("collected=%zu\n",gc_collect(h));
    gc_remove_root(h,root);
    printf("collected after root removal=%zu\n",gc_collect(h));
    gc_free(h);
    return 0;
}
