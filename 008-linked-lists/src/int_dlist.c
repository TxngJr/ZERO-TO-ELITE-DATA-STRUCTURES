#include "int_dlist.h"

#include <stdlib.h>

typedef struct DNode {
    int value;
    struct DNode *prev;
    struct DNode *next;
} DNode;

struct IntDList {
    DNode *head;
    DNode *tail;
    size_t size;
};

static DNode *make_node(int value) {
    DNode *n=malloc(sizeof *n);
    if(n){n->value=value;n->prev=NULL;n->next=NULL;}
    return n;
}

IntDList *int_dlist_create(void){return calloc(1,sizeof(IntDList));}

void int_dlist_free(IntDList *list){
    if(!list)return;
    DNode *n=list->head;
    while(n){DNode *next=n->next;free(n);n=next;}
    free(list);
}

size_t int_dlist_size(const IntDList *list){return list?list->size:0;}

static DNode *node_at(const IntDList *list,size_t index){
    if(!list||index>=list->size)return NULL;
    if(index < list->size/2){
        DNode *n=list->head;
        for(size_t i=0;i<index;++i)n=n->next;
        return n;
    }
    DNode *n=list->tail;
    for(size_t i=list->size-1;i>index;--i)n=n->prev;
    return n;
}

bool int_dlist_push_front(IntDList *list,int value){
    if(!list)return false;
    DNode *n=make_node(value); if(!n)return false;
    n->next=list->head;
    if(list->head)list->head->prev=n; else list->tail=n;
    list->head=n; ++list->size; return true;
}

bool int_dlist_push_back(IntDList *list,int value){
    if(!list)return false;
    DNode *n=make_node(value); if(!n)return false;
    n->prev=list->tail;
    if(list->tail)list->tail->next=n; else list->head=n;
    list->tail=n; ++list->size; return true;
}

bool int_dlist_pop_front(IntDList *list,int *out){
    if(!list||!list->head)return false;
    DNode *v=list->head;
    list->head=v->next;
    if(list->head)list->head->prev=NULL; else list->tail=NULL;
    if(out)*out=v->value;
    free(v); --list->size; return true;
}

bool int_dlist_pop_back(IntDList *list,int *out){
    if(!list||!list->tail)return false;
    DNode *v=list->tail;
    list->tail=v->prev;
    if(list->tail)list->tail->next=NULL; else list->head=NULL;
    if(out)*out=v->value;
    free(v); --list->size; return true;
}

bool int_dlist_get(const IntDList *list,size_t index,int *out){
    if(!out)return false;
    DNode *n=node_at(list,index); if(!n)return false;
    *out=n->value; return true;
}

bool int_dlist_insert(IntDList *list,size_t index,int value){
    if(!list||index>list->size)return false;
    if(index==0)return int_dlist_push_front(list,value);
    if(index==list->size)return int_dlist_push_back(list,value);
    DNode *next=node_at(list,index);
    DNode *prev=next->prev;
    DNode *n=make_node(value); if(!n)return false;
    n->prev=prev;n->next=next;prev->next=n;next->prev=n;
    ++list->size;return true;
}

bool int_dlist_erase(IntDList *list,size_t index,int *out){
    if(!list||index>=list->size)return false;
    if(index==0)return int_dlist_pop_front(list,out);
    if(index+1==list->size)return int_dlist_pop_back(list,out);
    DNode *v=node_at(list,index);
    v->prev->next=v->next;
    v->next->prev=v->prev;
    if(out)*out=v->value;
    free(v);--list->size;return true;
}

bool int_dlist_validate(const IntDList *list){
    if(!list)return false;
    if(list->size==0)return !list->head&&!list->tail;
    if(!list->head||!list->tail||list->head->prev||list->tail->next)return false;

    size_t forward=0;
    const DNode *n=list->head,*prev=NULL;
    while(n&&forward<=list->size){
        if(n->prev!=prev)return false;
        prev=n;n=n->next;++forward;
    }
    if(n||forward!=list->size||prev!=list->tail)return false;

    size_t backward=0;
    const DNode *next=NULL;
    n=list->tail;
    while(n&&backward<=list->size){
        if(n->next!=next)return false;
        next=n;n=n->prev;++backward;
    }
    return n==NULL&&backward==list->size&&next==list->head;
}
