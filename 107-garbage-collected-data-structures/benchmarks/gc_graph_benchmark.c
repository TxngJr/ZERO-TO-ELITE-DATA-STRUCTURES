#include "gc_graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum{NODES=100000};

static double elapsed(struct timespec a,struct timespec b){
    return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;
}

int main(void){
    GcHeap*h=gc_create(NODES);
    GcHandle*handles=malloc((size_t)NODES*sizeof(*handles));
    if(!h||!handles)return 1;

    for(int i=0;i<NODES;++i){
        if(!gc_alloc(h,i,&handles[i]))return 2;
        if(i>0&&!gc_set_edge(h,handles[i],0U,handles[i-1]))return 3;
    }
    if(!gc_add_root(h,handles[NODES-1]))return 4;

    struct timespec a,b,c;
    timespec_get(&a,TIME_UTC);
    size_t first=gc_collect(h);
    timespec_get(&b,TIME_UTC);
    if(!gc_remove_root(h,handles[NODES-1]))return 5;
    size_t second=gc_collect(h);
    timespec_get(&c,TIME_UTC);

    printf("nodes=%d keep_collect=%.6f reclaimed_first=%zu "
           "sweep_collect=%.6f reclaimed_second=%zu\n",
           NODES,elapsed(a,b),first,elapsed(b,c),second);

    free(handles);
    gc_free(h);
    return 0;
}
