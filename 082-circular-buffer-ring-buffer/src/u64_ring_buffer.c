#include "u64_ring_buffer.h"
#include <stdlib.h>
struct U64RingBuffer{uint64_t*data;size_t capacity,head,size;};
U64RingBuffer*u64_ring_buffer_create(size_t c){if(c==0||c>SIZE_MAX/sizeof(uint64_t))return NULL;U64RingBuffer*b=calloc(1,sizeof*b);if(!b)return NULL;b->data=malloc(c*sizeof*b->data);if(!b->data){free(b);return NULL;}b->capacity=c;return b;}
void u64_ring_buffer_free(U64RingBuffer*b){if(!b)return;free(b->data);free(b);}
size_t u64_ring_buffer_capacity(const U64RingBuffer*b){return b?b->capacity:0;}
size_t u64_ring_buffer_size(const U64RingBuffer*b){return b?b->size:0;}
bool u64_ring_buffer_is_empty(const U64RingBuffer*b){return !b||b->size==0;}
bool u64_ring_buffer_is_full(const U64RingBuffer*b){return b&&b->size==b->capacity;}
static size_t phys(const U64RingBuffer*b,size_t logical){const size_t first=b->head;const size_t remain=b->capacity-first;return logical<remain?first+logical:logical-remain;}
bool u64_ring_buffer_push(U64RingBuffer*b,uint64_t v){if(!b||b->size==b->capacity)return false;const size_t tail=phys(b,b->size);b->data[tail]=v;++b->size;return true;}
bool u64_ring_buffer_pop(U64RingBuffer*b,uint64_t*out){if(!b||!out||b->size==0)return false;*out=b->data[b->head];b->head=(b->head+1==b->capacity)?0:b->head+1;--b->size;if(b->size==0)b->head=0;return true;}
bool u64_ring_buffer_front(const U64RingBuffer*b,uint64_t*out){if(!b||!out||b->size==0)return false;*out=b->data[b->head];return true;}
bool u64_ring_buffer_back(const U64RingBuffer*b,uint64_t*out){if(!b||!out||b->size==0)return false;*out=b->data[phys(b,b->size-1)];return true;}
bool u64_ring_buffer_get(const U64RingBuffer*b,size_t i,uint64_t*out){if(!b||!out||i>=b->size)return false;*out=b->data[phys(b,i)];return true;}
void u64_ring_buffer_clear(U64RingBuffer*b){if(!b)return;b->head=0;b->size=0;}
bool u64_ring_buffer_validate(const U64RingBuffer*b){if(!b||!b->data||b->capacity==0||b->head>=b->capacity||b->size>b->capacity)return false;if(b->size==0&&b->head!=0)return false;for(size_t i=0;i<b->size;++i)if(phys(b,i)>=b->capacity)return false;return true;}
