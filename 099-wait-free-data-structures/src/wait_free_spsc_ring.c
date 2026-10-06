#include "wait_free_spsc_ring.h"
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
struct WaitFreeSpscRing{size_t capacity;int*data;_Atomic size_t head,tail;};
WaitFreeSpscRing*wfr_create(size_t cap){if(cap<2U||cap>SIZE_MAX/sizeof(int))return NULL;WaitFreeSpscRing*r=calloc(1,sizeof*r);if(!r)return NULL;r->data=malloc(cap*sizeof(int));if(!r->data){free(r);return NULL;}r->capacity=cap;atomic_init(&r->head,0U);atomic_init(&r->tail,0U);return r;}
void wfr_free(WaitFreeSpscRing*r){if(!r)return;free(r->data);free(r);}
bool wfr_platform_lock_free(const WaitFreeSpscRing*r){return r&&atomic_is_lock_free(&r->head)&&atomic_is_lock_free(&r->tail);}
size_t wfr_usable_capacity(const WaitFreeSpscRing*r){return r&&r->capacity?r->capacity-1U:0U;}
static size_t next_index(size_t i,size_t cap){++i;return i==cap?0U:i;}
bool wfr_try_push(WaitFreeSpscRing*r,int value,bool*out){if(!r)return false;size_t tail=atomic_load_explicit(&r->tail,memory_order_relaxed),next=next_index(tail,r->capacity),head=atomic_load_explicit(&r->head,memory_order_acquire);if(next==head){if(out)*out=false;return true;}r->data[tail]=value;atomic_store_explicit(&r->tail,next,memory_order_release);if(out)*out=true;return true;}
bool wfr_try_pop(WaitFreeSpscRing*r,int*out_v,bool*out){if(!r||!out_v)return false;size_t head=atomic_load_explicit(&r->head,memory_order_relaxed),tail=atomic_load_explicit(&r->tail,memory_order_acquire);if(head==tail){if(out)*out=false;return true;}*out_v=r->data[head];atomic_store_explicit(&r->head,next_index(head,r->capacity),memory_order_release);if(out)*out=true;return true;}
bool wfr_validate_quiescent(const WaitFreeSpscRing*r){if(!r||!r->data||r->capacity<2U)return false;size_t h=atomic_load_explicit(&r->head,memory_order_acquire),t=atomic_load_explicit(&r->tail,memory_order_acquire);return h<r->capacity&&t<r->capacity;}
