#include "false_sharing.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>

typedef _Atomic uint64_t AtomicCounter;

struct FsCounterArray {
    unsigned char *raw;
    unsigned char *base;
    size_t allocation_bytes;
    size_t count;
    size_t stride;
    size_t line_size;
};

static bool power_of_two(size_t x){
    return x!=0U&&(x&(x-1U))==0U;
}

static AtomicCounter *counter_ptr(FsCounterArray *a,size_t index){
    return (AtomicCounter *)(void *)(a->base+index*a->stride);
}

static const AtomicCounter *counter_ptr_const(const FsCounterArray *a,size_t index){
    return (const AtomicCounter *)(const void *)(a->base+index*a->stride);
}

FsCounterArray *fs_counter_array_create(size_t count,size_t stride,size_t line_size){
    size_t atomic_alignment=_Alignof(AtomicCounter);
    if(count==0U||stride<sizeof(AtomicCounter)||!power_of_two(line_size)||
       line_size<atomic_alignment||stride%atomic_alignment!=0U||
       count>SIZE_MAX/stride)return NULL;

    size_t logical=count*stride;
    size_t slack=line_size-1U;
    if(logical>SIZE_MAX-slack)return NULL;
    size_t allocation=logical+slack;

    FsCounterArray*a=calloc(1,sizeof(*a));
    if(!a)return NULL;
    a->raw=malloc(allocation);
    if(!a->raw){free(a);return NULL;}

    uintptr_t raw=(uintptr_t)a->raw;
    uintptr_t mask=(uintptr_t)line_size-1U;
    if(raw>UINTPTR_MAX-mask){
        free(a->raw);free(a);return NULL;
    }
    uintptr_t aligned=(raw+mask)&~mask;
    a->base=(unsigned char*)aligned;
    a->allocation_bytes=allocation;
    a->count=count;a->stride=stride;a->line_size=line_size;

    for(size_t i=0;i<count;++i)
        atomic_init(counter_ptr(a,i),UINT64_C(0));
    return a;
}

void fs_counter_array_free(FsCounterArray*a){
    if(!a)return;
    free(a->raw);
    free(a);
}

size_t fs_counter_count(const FsCounterArray*a){return a?a->count:0U;}
size_t fs_counter_stride(const FsCounterArray*a){return a?a->stride:0U;}
size_t fs_line_size(const FsCounterArray*a){return a?a->line_size:0U;}
size_t fs_storage_bytes(const FsCounterArray*a){return a?a->count*a->stride:0U;}

static size_t relative_line(const FsCounterArray*a,size_t index){
    return (index*a->stride)/a->line_size;
}

bool fs_counters_share_line(const FsCounterArray*a,size_t x,size_t y,bool*out_share){
    if(!a||!out_share||x>=a->count||y>=a->count)return false;
    *out_share=relative_line(a,x)==relative_line(a,y);
    return true;
}

bool fs_line_occupancy(const FsCounterArray*a,size_t index,size_t*out_count){
    if(!a||!out_count||index>=a->count)return false;
    size_t line=relative_line(a,index),count=0U;
    for(size_t i=0;i<a->count;++i)
        if(relative_line(a,i)==line)++count;
    *out_count=count;
    return true;
}

bool fs_counter_load(const FsCounterArray*a,size_t index,uint64_t*out_value){
    if(!a||!out_value||index>=a->count)return false;
    *out_value=atomic_load_explicit(counter_ptr_const(a,index),memory_order_relaxed);
    return true;
}

bool fs_counter_add(FsCounterArray*a,size_t index,uint64_t delta){
    if(!a||index>=a->count)return false;
    atomic_fetch_add_explicit(counter_ptr(a,index),delta,memory_order_relaxed);
    return true;
}

void fs_counter_reset(FsCounterArray*a){
    if(!a)return;
    for(size_t i=0;i<a->count;++i)
        atomic_store_explicit(counter_ptr(a,i),UINT64_C(0),memory_order_relaxed);
}

typedef struct {
    FsCounterArray *array;
    size_t index;
    uint64_t iterations;
} Worker;

static void *worker_main(void*arg){
    Worker*w=arg;
    AtomicCounter*c=counter_ptr(w->array,w->index);
    for(uint64_t i=0;i<w->iterations;++i)
        atomic_fetch_add_explicit(c,UINT64_C(1),memory_order_relaxed);
    return NULL;
}

bool fs_parallel_increment(FsCounterArray*a,size_t thread_count,uint64_t iterations){
    if(!a||thread_count==0U||thread_count>a->count)return false;
    if(thread_count>SIZE_MAX/sizeof(pthread_t)||
       thread_count>SIZE_MAX/sizeof(Worker))return false;

    pthread_t*threads=malloc(thread_count*sizeof(*threads));
    Worker*workers=malloc(thread_count*sizeof(*workers));
    if(!threads||!workers){free(threads);free(workers);return false;}

    size_t created=0U;
    bool ok=true;
    for(size_t i=0;i<thread_count;++i){
        workers[i]=(Worker){a,i,iterations};
        if(pthread_create(&threads[i],NULL,worker_main,&workers[i])!=0){
            ok=false;
            break;
        }
        ++created;
    }
    for(size_t i=0;i<created;++i)
        if(pthread_join(threads[i],NULL)!=0)ok=false;

    free(workers);free(threads);
    return ok;
}

bool fs_counter_array_validate(const FsCounterArray*a){
    if(!a||!a->raw||!a->base||a->count==0U||
       a->stride<sizeof(AtomicCounter)||!power_of_two(a->line_size)||
       a->line_size<_Alignof(AtomicCounter)||
       a->stride%_Alignof(AtomicCounter)!=0U)return false;

    uintptr_t raw=(uintptr_t)a->raw,base=(uintptr_t)a->base;
    if(base<raw||base-raw>=a->line_size||base%a->line_size!=0U)return false;
    if(a->count>SIZE_MAX/a->stride)return false;
    size_t logical=a->count*a->stride;
    size_t offset=(size_t)(base-raw);
    if(offset>a->allocation_bytes||logical>a->allocation_bytes-offset)return false;

    for(size_t i=0;i<a->count;++i){
        uintptr_t p=(uintptr_t)(a->base+i*a->stride);
        if(p%_Alignof(AtomicCounter)!=0U)return false;
    }
    return true;
}
