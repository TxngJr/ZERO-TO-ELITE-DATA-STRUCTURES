#include "gc_graph.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum{CAPACITY=4096,NODES=3000};
static uint64_t state=UINT64_C(0xabcdef123456789);
static uint64_t rng(void){state^=state<<7;state^=state>>9;state^=state<<8;return state;}

int main(void){
    GcHeap*h=gc_create(CAPACITY);
    assert(h&&gc_validate(h));
    GcHandle handles[NODES];

    for(int i=0;i<NODES;++i)assert(gc_alloc(h,i,&handles[i]));

    for(int i=1;i<NODES;++i){
        if((rng()%100U)<70U){
            int p=(int)(rng()%(uint64_t)i);
            assert(gc_set_edge(h,handles[i],0U,handles[p]));
        }
        if((rng()%100U)<35U){
            int p=(int)(rng()%(uint64_t)i);
            assert(gc_set_edge(h,handles[i],1U,handles[p]));
        }
    }

    GcHandle cycle_a,cycle_b;
    assert(gc_alloc(h,111,&cycle_a));
    assert(gc_alloc(h,222,&cycle_b));
    assert(gc_set_edge(h,cycle_a,0U,cycle_b));
    assert(gc_set_edge(h,cycle_b,0U,cycle_a));

    unsigned char*root=calloc(NODES,1U);
    unsigned char*reach=calloc(NODES,1U);
    size_t*stack=malloc((size_t)NODES*sizeof(*stack));
    assert(root&&reach&&stack);

    for(int k=0;k<120;++k){
        int i=(int)(rng()%NODES);
        root[i]=1U;
        assert(gc_add_root(h,handles[i]));
    }

    size_t top=0U;
    for(int i=0;i<NODES;++i)
        if(root[i]){reach[i]=1U;stack[top++]=(size_t)i;}

    while(top){
        size_t i=stack[--top];
        for(size_t edge=0;edge<2U;++edge){
            GcHandle x;
            assert(gc_get_edge(h,handles[i],edge,&x));
            if(gc_handle_is_null(x))continue;
            if(x.index<(size_t)NODES&&!reach[x.index]){
                reach[x.index]=1U;
                stack[top++]=x.index;
            }
        }
    }

    size_t expected=0U;
    for(int i=0;i<NODES;++i)if(reach[i])++expected;

    size_t before=gc_live_count(h);
    size_t collected=gc_collect(h);
    assert(before-collected==expected);
    assert(!gc_is_alive(h,cycle_a));
    assert(!gc_is_alive(h,cycle_b));

    for(int i=0;i<NODES;++i)
        assert(gc_is_alive(h,handles[i])==(reach[i]!=0U));
    assert(gc_validate(h));

    for(int i=0;i<NODES;++i){
        if(root[i]&&(i%2==0))assert(gc_remove_root(h,handles[i]));
    }
    gc_collect(h);
    assert(gc_validate(h));

    GcHandle stale=cycle_a,reused;
    assert(gc_alloc(h,999,&reused));
    assert(!gc_is_alive(h,stale));
    int value=0;
    assert(gc_get_value(h,reused,&value)&&value==999);

    printf("GC tests passed; live=%zu roots=%zu\n",
           gc_live_count(h),gc_root_count(h));
    free(stack);free(root);free(reach);
    gc_free(h);
    return 0;
}
