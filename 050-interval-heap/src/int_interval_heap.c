#include "int_interval_heap.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int64_t low;
    int64_t high;
} IntervalHeapNode;

struct IntIntervalHeap {
    IntervalHeapNode *nodes;
    size_t size;
    size_t capacity;
};

static size_t node_count_for(size_t element_count) {
    return element_count/2 + element_count%2;
}

static bool reserve_nodes(
    IntIntervalHeap *heap,
    size_t needed
) {
    if(needed<=heap->capacity)return true;

    size_t capacity=heap->capacity==0?8:heap->capacity;

    while(capacity<needed) {
        if(capacity>SIZE_MAX/2) {
            capacity=needed;
            break;
        }
        capacity*=2;
    }

    if(capacity>SIZE_MAX/sizeof *heap->nodes)return false;

    IntervalHeapNode *next=realloc(
        heap->nodes,
        capacity*sizeof *next
    );

    if(next==NULL)return false;

    heap->nodes=next;
    heap->capacity=capacity;
    return true;
}

IntIntervalHeap *int_interval_heap_create(void) {
    return calloc(1,sizeof(IntIntervalHeap));
}

void int_interval_heap_free(IntIntervalHeap *heap) {
    if(heap==NULL)return;
    free(heap->nodes);
    free(heap);
}

size_t int_interval_heap_size(const IntIntervalHeap *heap) {
    return heap==NULL?0:heap->size;
}

bool int_interval_heap_is_empty(const IntIntervalHeap *heap) {
    return heap==NULL||heap->size==0;
}

static void sift_up_min(
    IntIntervalHeap *heap,
    size_t index,
    int64_t value
) {
    while(index>0) {
        const size_t parent=(index-1)/2;

        if(heap->nodes[parent].low<=value)break;

        heap->nodes[index].low=
            heap->nodes[parent].low;
        index=parent;
    }

    heap->nodes[index].low=value;
}

static void sift_up_max(
    IntIntervalHeap *heap,
    size_t index,
    int64_t value
) {
    while(index>0) {
        const size_t parent=(index-1)/2;

        if(heap->nodes[parent].high>=value)break;

        heap->nodes[index].high=
            heap->nodes[parent].high;
        index=parent;
    }

    heap->nodes[index].high=value;
}

bool int_interval_heap_insert(
    IntIntervalHeap *heap,
    int64_t value
) {
    if(heap==NULL||heap->size==SIZE_MAX)return false;

    if(heap->size==0) {
        if(!reserve_nodes(heap,1))return false;

        heap->nodes[0]=(IntervalHeapNode){
            .low=value,
            .high=value
        };
        heap->size=1;
        return true;
    }

    const size_t old_size=heap->size;

    if((old_size&1U)!=0U) {
        /* Complete the existing final singleton node. */
        const size_t index=old_size/2;
        const int64_t existing=heap->nodes[index].low;

        if(value<existing) {
            heap->nodes[index].low=value;
            heap->nodes[index].high=existing;
            sift_up_min(heap,index,value);
        } else {
            heap->nodes[index].low=existing;
            heap->nodes[index].high=value;
            sift_up_max(heap,index,value);
        }

        heap->size=old_size+1;
        return true;
    }

    /* Create a new singleton child. */
    const size_t index=old_size/2;

    if(!reserve_nodes(heap,index+1))return false;

    const size_t parent=(index-1)/2;
    const int64_t parent_low=heap->nodes[parent].low;
    const int64_t parent_high=heap->nodes[parent].high;

    if(value<parent_low) {
        heap->nodes[index]=(IntervalHeapNode){
            .low=parent_low,
            .high=parent_low
        };
        sift_up_min(heap,parent,value);
    } else if(value>parent_high) {
        heap->nodes[index]=(IntervalHeapNode){
            .low=parent_high,
            .high=parent_high
        };
        sift_up_max(heap,parent,value);
    } else {
        heap->nodes[index]=(IntervalHeapNode){
            .low=value,
            .high=value
        };
    }

    heap->size=old_size+1;
    return true;
}

bool int_interval_heap_peek_min(
    const IntIntervalHeap *heap,
    int64_t *out_value
) {
    if(heap==NULL||out_value==NULL||heap->size==0) {
        return false;
    }

    *out_value=heap->nodes[0].low;
    return true;
}

bool int_interval_heap_peek_max(
    const IntIntervalHeap *heap,
    int64_t *out_value
) {
    if(heap==NULL||out_value==NULL||heap->size==0) {
        return false;
    }

    *out_value=heap->nodes[0].high;
    return true;
}

static bool is_singleton_node(
    const IntIntervalHeap *heap,
    size_t index
) {
    const size_t count=node_count_for(heap->size);

    return (heap->size&1U)!=0U &&
           index+1==count;
}

static void sift_down_min(
    IntIntervalHeap *heap,
    int64_t value
) {
    const size_t count=node_count_for(heap->size);
    size_t index=0;

    for(;;) {
        const size_t left=index*2+1;

        if(left>=count)break;

        size_t child=left;
        const size_t right=left+1;

        if(right<count &&
           heap->nodes[right].low<
               heap->nodes[left].low) {
            child=right;
        }

        if(value<=heap->nodes[child].low)break;

        heap->nodes[index].low=
            heap->nodes[child].low;

        if(!is_singleton_node(heap,child) &&
           value>heap->nodes[child].high) {
            const int64_t tmp=value;
            value=heap->nodes[child].high;
            heap->nodes[child].high=tmp;
        }

        index=child;
    }

    heap->nodes[index].low=value;

    if(is_singleton_node(heap,index)) {
        heap->nodes[index].high=value;
    }
}

static void sift_down_max(
    IntIntervalHeap *heap,
    int64_t value
) {
    const size_t count=node_count_for(heap->size);
    size_t index=0;

    for(;;) {
        const size_t left=index*2+1;

        if(left>=count)break;

        size_t child=left;
        const size_t right=left+1;

        if(right<count &&
           heap->nodes[right].high>
               heap->nodes[left].high) {
            child=right;
        }

        if(value>=heap->nodes[child].high)break;

        heap->nodes[index].high=
            heap->nodes[child].high;

        if(!is_singleton_node(heap,child) &&
           value<heap->nodes[child].low) {
            const int64_t tmp=value;
            value=heap->nodes[child].low;
            heap->nodes[child].low=tmp;
        }

        index=child;
    }

    heap->nodes[index].high=value;

    if(is_singleton_node(heap,index)) {
        heap->nodes[index].low=value;
    }
}

bool int_interval_heap_pop_min(
    IntIntervalHeap *heap,
    int64_t *out_value
) {
    if(heap==NULL||out_value==NULL||heap->size==0) {
        return false;
    }

    *out_value=heap->nodes[0].low;

    if(heap->size==1) {
        heap->size=0;
        return true;
    }

    if(heap->size==2) {
        heap->nodes[0].low=heap->nodes[0].high;
        heap->size=1;
        return true;
    }

    const size_t old_size=heap->size;
    const size_t last=(old_size-1)/2;
    int64_t replacement=0;

    if((old_size&1U)!=0U) {
        replacement=heap->nodes[last].low;
    } else {
        replacement=heap->nodes[last].high;

        const int64_t remain=heap->nodes[last].low;
        heap->nodes[last].low=remain;
        heap->nodes[last].high=remain;
    }

    heap->size=old_size-1;
    sift_down_min(heap,replacement);
    return true;
}

bool int_interval_heap_pop_max(
    IntIntervalHeap *heap,
    int64_t *out_value
) {
    if(heap==NULL||out_value==NULL||heap->size==0) {
        return false;
    }

    *out_value=heap->nodes[0].high;

    if(heap->size==1) {
        heap->size=0;
        return true;
    }

    if(heap->size==2) {
        heap->nodes[0].high=heap->nodes[0].low;
        heap->size=1;
        return true;
    }

    const size_t old_size=heap->size;
    const size_t last=(old_size-1)/2;
    int64_t replacement=0;

    if((old_size&1U)!=0U) {
        replacement=heap->nodes[last].low;
    } else {
        replacement=heap->nodes[last].low;

        const int64_t remain=heap->nodes[last].high;
        heap->nodes[last].low=remain;
        heap->nodes[last].high=remain;
    }

    heap->size=old_size-1;
    sift_down_max(heap,replacement);
    return true;
}

bool int_interval_heap_validate(
    const IntIntervalHeap *heap
) {
    if(heap==NULL)return false;

    if(heap->size==0)return true;

    const size_t count=node_count_for(heap->size);

    if(heap->nodes==NULL||count>heap->capacity)return false;

    for(size_t i=0;i<count;++i) {
        const bool singleton=
            (heap->size&1U)!=0U&&i+1==count;

        if(heap->nodes[i].low>heap->nodes[i].high) {
            return false;
        }

        if(singleton &&
           heap->nodes[i].low!=heap->nodes[i].high) {
            return false;
        }

        if(i>0) {
            const size_t parent=(i-1)/2;

            if(heap->nodes[parent].low>
                   heap->nodes[i].low ||
               heap->nodes[i].high>
                   heap->nodes[parent].high) {
                return false;
            }
        }
    }

    return true;
}
