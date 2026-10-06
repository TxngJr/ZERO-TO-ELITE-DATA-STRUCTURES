#include "lock_free_stack.h"
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
enum{LFS_EMPTY=SIZE_MAX};
typedef struct{int value;size_t next;}LFNode;
struct LockFreeStack{size_t capacity;LFNode*nodes;_Atomic size_t next_slot,head,size,push_failures,pop_failures;};
LockFreeStack*lfs_create(size_t cap){if(cap==0||cap>SIZE_MAX/sizeof(LFNode))return NULL;LockFreeStack*s=calloc(1,sizeof*s);if(!s)return NULL;s->nodes=malloc(cap*sizeof(*s->nodes));if(!s->nodes){free(s);return NULL;}s->capacity=cap;atomic_init(&s->next_slot,0U);atomic_init(&s->head,LFS_EMPTY);atomic_init(&s->size,0U);atomic_init(&s->push_failures,0U);atomic_init(&s->pop_failures,0U);return s;}
void lfs_free(LockFreeStack*s){if(!s)return;free(s->nodes);free(s);}
bool lfs_platform_lock_free(const LockFreeStack*s){return s&&atomic_is_lock_free(&s->head)&&atomic_is_lock_free(&s->next_slot)&&atomic_is_lock_free(&s->size)&&atomic_is_lock_free(&s->push_failures)&&atomic_is_lock_free(&s->pop_failures);}
bool lfs_push(LockFreeStack*s,int value,bool*out){
    if(!s)return false;size_t idx=atomic_load_explicit(&s->next_slot,memory_order_relaxed);
    for(;;){if(idx>=s->capacity){if(out)*out=false;return true;}size_t desired=idx+1U;if(atomic_compare_exchange_weak_explicit(&s->next_slot,&idx,desired,memory_order_relaxed,memory_order_relaxed))break;}
    s->nodes[idx].value=value;
    atomic_fetch_add_explicit(&s->size,1U,memory_order_relaxed);
    size_t observed=atomic_load_explicit(&s->head,memory_order_acquire);
    for(;;){s->nodes[idx].next=observed;if(atomic_compare_exchange_weak_explicit(&s->head,&observed,idx,memory_order_release,memory_order_acquire))break;atomic_fetch_add_explicit(&s->push_failures,1U,memory_order_relaxed);}
    if(out)*out=true;return true;
}
bool lfs_pop(LockFreeStack*s,int*out_v,bool*out){
    if(!s||!out_v)return false;size_t observed=atomic_load_explicit(&s->head,memory_order_acquire);
    for(;;){if(observed==LFS_EMPTY){if(out)*out=false;return true;}if(observed>=s->capacity)return false;size_t next=s->nodes[observed].next;if(atomic_compare_exchange_weak_explicit(&s->head,&observed,next,memory_order_acq_rel,memory_order_acquire)){*out_v=s->nodes[observed].value;atomic_fetch_sub_explicit(&s->size,1U,memory_order_relaxed);if(out)*out=true;return true;}atomic_fetch_add_explicit(&s->pop_failures,1U,memory_order_relaxed);}
}
size_t lfs_size_approx(const LockFreeStack*s){return s?atomic_load_explicit(&s->size,memory_order_relaxed):0U;}
size_t lfs_reserved_slots(const LockFreeStack*s){if(!s)return 0U;size_t n=atomic_load_explicit(&s->next_slot,memory_order_relaxed);return n<s->capacity?n:s->capacity;}
size_t lfs_push_cas_failures(const LockFreeStack*s){return s?atomic_load_explicit(&s->push_failures,memory_order_relaxed):0U;}
size_t lfs_pop_cas_failures(const LockFreeStack*s){return s?atomic_load_explicit(&s->pop_failures,memory_order_relaxed):0U;}
bool lfs_validate_quiescent(const LockFreeStack*s){if(!s||!s->nodes||s->capacity==0)return false;size_t reserved=lfs_reserved_slots(s);unsigned char*seen=calloc(reserved?reserved:1U,1U);if(!seen)return false;size_t count=0,cur=atomic_load_explicit(&s->head,memory_order_acquire);bool ok=true;while(cur!=LFS_EMPTY){if(cur>=reserved||seen[cur]){ok=false;break;}seen[cur]=1U;++count;if(count>reserved){ok=false;break;}cur=s->nodes[cur].next;}if(ok)ok=count==lfs_size_approx(s);free(seen);return ok;}
