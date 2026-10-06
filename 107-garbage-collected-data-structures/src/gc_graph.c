#include "gc_graph.h"
#include <stdlib.h>

typedef struct {
    int value;
    GcHandle edges[2];
    uint64_t generation;
    unsigned char alive;
    unsigned char marked;
    unsigned char root;
    unsigned char retired;
} GcObject;

struct GcHeap {
    size_t capacity;
    size_t live_count;
    size_t root_count;
    size_t retired_count;
    size_t free_top;
    GcObject *objects;
    size_t *free_stack;
    size_t *mark_stack;
};

GcHandle gc_null_handle(void){return (GcHandle){SIZE_MAX,0U};}
bool gc_handle_is_null(GcHandle h){return h.index==SIZE_MAX;}

GcHeap*gc_create(size_t capacity){
    if(capacity==0U||capacity>SIZE_MAX/sizeof(GcObject)||
       capacity>SIZE_MAX/sizeof(size_t))return NULL;
    GcHeap*h=calloc(1,sizeof(*h));
    if(!h)return NULL;

    h->objects=calloc(capacity,sizeof(*h->objects));
    h->free_stack=malloc(capacity*sizeof(*h->free_stack));
    h->mark_stack=malloc(capacity*sizeof(*h->mark_stack));
    if(!h->objects||!h->free_stack||!h->mark_stack){
        free(h->objects);free(h->free_stack);free(h->mark_stack);free(h);
        return NULL;
    }

    h->capacity=capacity;
    h->free_top=capacity;
    for(size_t i=0;i<capacity;++i){
        h->free_stack[i]=capacity-1U-i;
        h->objects[i].generation=1U;
        h->objects[i].edges[0]=gc_null_handle();
        h->objects[i].edges[1]=gc_null_handle();
    }
    return h;
}

void gc_free(GcHeap*h){
    if(!h)return;
    free(h->objects);
    free(h->free_stack);
    free(h->mark_stack);
    free(h);
}

static bool valid_handle(const GcHeap*h,GcHandle x){
    return h&&!gc_handle_is_null(x)&&x.index<h->capacity&&
           h->objects[x.index].alive&&
           h->objects[x.index].generation==x.generation;
}

bool gc_is_alive(const GcHeap*h,GcHandle x){return valid_handle(h,x);}

bool gc_alloc(GcHeap*h,int value,GcHandle*out){
    if(!h||!out||h->free_top==0U)return false;
    size_t idx=h->free_stack[--h->free_top];
    GcObject*o=&h->objects[idx];
    if(o->alive||o->retired){++h->free_top;return false;}

    o->value=value;
    o->edges[0]=gc_null_handle();
    o->edges[1]=gc_null_handle();
    o->marked=0U;
    o->root=0U;
    o->alive=1U;
    ++h->live_count;
    *out=(GcHandle){idx,o->generation};
    return true;
}

bool gc_get_value(const GcHeap*h,GcHandle x,int*out){
    if(!out||!valid_handle(h,x))return false;
    *out=h->objects[x.index].value;
    return true;
}

bool gc_set_value(GcHeap*h,GcHandle x,int value){
    if(!valid_handle(h,x))return false;
    h->objects[x.index].value=value;
    return true;
}

bool gc_set_edge(GcHeap*h,GcHandle from,size_t edge_index,GcHandle to){
    if(!valid_handle(h,from)||edge_index>=2U)return false;
    if(!gc_handle_is_null(to)&&!valid_handle(h,to))return false;
    h->objects[from.index].edges[edge_index]=to;
    return true;
}

bool gc_get_edge(const GcHeap*h,GcHandle from,size_t edge_index,GcHandle*out){
    if(!out||!valid_handle(h,from)||edge_index>=2U)return false;
    *out=h->objects[from.index].edges[edge_index];
    return true;
}

bool gc_add_root(GcHeap*h,GcHandle x){
    if(!valid_handle(h,x))return false;
    GcObject*o=&h->objects[x.index];
    if(!o->root){o->root=1U;++h->root_count;}
    return true;
}

bool gc_remove_root(GcHeap*h,GcHandle x){
    if(!valid_handle(h,x))return false;
    GcObject*o=&h->objects[x.index];
    if(o->root){o->root=0U;--h->root_count;}
    return true;
}

size_t gc_collect(GcHeap*h){
    if(!h)return 0U;
    size_t top=0U;

    for(size_t i=0;i<h->capacity;++i){
        GcObject*o=&h->objects[i];
        if(o->alive){
            o->marked=0U;
            if(o->root){
                o->marked=1U;
                h->mark_stack[top++]=i;
            }
        }
    }

    while(top){
        size_t idx=h->mark_stack[--top];
        GcObject*o=&h->objects[idx];
        for(size_t e=0;e<2U;++e){
            GcHandle child=o->edges[e];
            if(valid_handle(h,child)){
                GcObject*c=&h->objects[child.index];
                if(!c->marked){
                    c->marked=1U;
                    h->mark_stack[top++]=child.index;
                }
            }
        }
    }

    size_t collected=0U;
    for(size_t i=0;i<h->capacity;++i){
        GcObject*o=&h->objects[i];
        if(o->alive&&!o->marked){
            o->alive=0U;
            o->root=0U;
            o->marked=0U;
            o->edges[0]=gc_null_handle();
            o->edges[1]=gc_null_handle();

            if(o->generation==UINT64_MAX){
                o->retired=1U;
                ++h->retired_count;
            }else{
                ++o->generation;
                h->free_stack[h->free_top++]=i;
            }

            --h->live_count;
            ++collected;
        }else if(o->alive){
            o->marked=0U;
        }
    }
    return collected;
}

size_t gc_live_count(const GcHeap*h){return h?h->live_count:0U;}
size_t gc_root_count(const GcHeap*h){return h?h->root_count:0U;}

bool gc_validate(const GcHeap*h){
    if(!h||!h->objects||!h->free_stack||!h->mark_stack||
       h->free_top>h->capacity||h->live_count>h->capacity||
       h->retired_count>h->capacity||h->root_count>h->live_count)return false;

    unsigned char*seen=calloc(h->capacity,1U);
    if(!seen)return false;
    size_t live=0U,roots=0U,retired=0U;

    for(size_t i=0;i<h->capacity;++i){
        const GcObject*o=&h->objects[i];
        if(o->generation==0U){free(seen);return false;}
        if(o->retired){
            if(o->alive){free(seen);return false;}
            ++retired;
        }
        if(o->alive){
            ++live;
            if(o->root)++roots;
            for(size_t e=0;e<2U;++e){
                GcHandle x=o->edges[e];
                if(!gc_handle_is_null(x)&&!valid_handle(h,x)){
                    free(seen);
                    return false;
                }
            }
        }
    }

    for(size_t i=0;i<h->free_top;++i){
        size_t idx=h->free_stack[i];
        if(idx>=h->capacity||seen[idx]||h->objects[idx].alive||
           h->objects[idx].retired){
            free(seen);
            return false;
        }
        seen[idx]=1U;
    }

    bool ok=live==h->live_count&&roots==h->root_count&&
            retired==h->retired_count&&
            h->free_top+live+retired==h->capacity;
    free(seen);
    return ok;
}
