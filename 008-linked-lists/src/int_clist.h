#ifndef INT_CLIST_H
#define INT_CLIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntCList IntCList;

IntCList *int_clist_create(void);
void int_clist_free(IntCList *list);
size_t int_clist_size(const IntCList *list);
bool int_clist_push_back(IntCList *list,int value);
bool int_clist_pop_front(IntCList *list,int *out);
bool int_clist_rotate_left(IntCList *list);
bool int_clist_get(const IntCList *list,size_t index,int *out);
bool int_clist_validate(const IntCList *list);

#endif
