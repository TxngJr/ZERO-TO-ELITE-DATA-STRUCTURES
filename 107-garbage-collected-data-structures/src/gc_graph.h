#ifndef GC_GRAPH_H
#define GC_GRAPH_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct GcHeap GcHeap;
typedef struct { size_t index; uint64_t generation; } GcHandle;
GcHandle gc_null_handle(void);
bool gc_handle_is_null(GcHandle handle);
GcHeap *gc_create(size_t capacity);
void gc_free(GcHeap *heap);
bool gc_alloc(GcHeap *heap,int value,GcHandle *out_handle);
bool gc_get_value(const GcHeap *heap,GcHandle handle,int *out_value);
bool gc_set_value(GcHeap *heap,GcHandle handle,int value);
bool gc_set_edge(GcHeap *heap,GcHandle from,size_t edge_index,GcHandle to);
bool gc_get_edge(const GcHeap *heap,GcHandle from,size_t edge_index,GcHandle *out_to);
bool gc_add_root(GcHeap *heap,GcHandle handle);
bool gc_remove_root(GcHeap *heap,GcHandle handle);
size_t gc_collect(GcHeap *heap);
size_t gc_live_count(const GcHeap *heap);
size_t gc_root_count(const GcHeap *heap);
bool gc_is_alive(const GcHeap *heap,GcHandle handle);
bool gc_validate(const GcHeap *heap);
#endif
