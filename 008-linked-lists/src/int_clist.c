#include "int_clist.h"

#include <stdlib.h>

typedef struct CNode {
    int value;
    struct CNode *next;
} CNode;

struct IntCList {
    CNode *tail;
    size_t size;
};

IntCList *int_clist_create(void){return calloc(1,sizeof(IntCList));}

void int_clist_free(IntCList *list){
    if(!list)return;
    if(list->tail){
        CNode *head=list->tail->next;
        list->tail->next=NULL;
        CNode *n=head;
        while(n){CNode *next=n->next;free(n);n=next;}
    }
    free(list);
}

size_t int_clist_size(const IntCList *list){return list?list->size:0;}

bool int_clist_push_back(IntCList *list,int value){
    if(!list)return false;
    CNode *n=malloc(sizeof *n); if(!n)return false;
    n->value=value;
    if(!list->tail){
        n->next=n;
        list->tail=n;
    }else{
        n->next=list->tail->next;
        list->tail->next=n;
        list->tail=n;
    }
    ++list->size;return true;
}

bool int_clist_pop_front(IntCList *list,int *out){
    if(!list||!list->tail)return false;
    CNode *head=list->tail->next;
    if(out)*out=head->value;
    if(head==list->tail)list->tail=NULL;
    else list->tail->next=head->next;
    free(head);--list->size;return true;
}

bool int_clist_rotate_left(IntCList *list){
    if(!list||!list->tail)return false;
    list->tail=list->tail->next;
    return true;
}

bool int_clist_get(const IntCList *list,size_t index,int *out){
    if(!list||!list->tail||index>=list->size||!out)return false;
    CNode *n=list->tail->next;
    for(size_t i=0;i<index;++i)n=n->next;
    *out=n->value;return true;
}

bool int_clist_validate(const IntCList *list){
    if(!list)return false;
    if(list->size==0)return list->tail==NULL;
    if(!list->tail||!list->tail->next)return false;
    const CNode *head=list->tail->next;
    const CNode *n=head;
    for(size_t i=0;i<list->size;++i){
        if(!n)return false;
        n=n->next;
        if(i+1<list->size&&n==head)return false;
    }
    return n==head;
}
