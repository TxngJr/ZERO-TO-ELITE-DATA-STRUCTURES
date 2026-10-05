#ifndef INT_DLIST_H
#define INT_DLIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntDList IntDList;

IntDList *int_dlist_create(void);
void int_dlist_free(IntDList *list);
size_t int_dlist_size(const IntDList *list);
bool int_dlist_push_front(IntDList *list,int value);
bool int_dlist_push_back(IntDList *list,int value);
bool int_dlist_pop_front(IntDList *list,int *out);
bool int_dlist_pop_back(IntDList *list,int *out);
bool int_dlist_get(const IntDList *list,size_t index,int *out);
bool int_dlist_insert(IntDList *list,size_t index,int value);
bool int_dlist_erase(IntDList *list,size_t index,int *out);
bool int_dlist_validate(const IntDList *list);

#endif
