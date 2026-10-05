#ifndef INT_SLIST_H
#define INT_SLIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntSList IntSList;

IntSList *int_slist_create(void);
void int_slist_free(IntSList *list);
size_t int_slist_size(const IntSList *list);

bool int_slist_push_front(IntSList *list, int value);
bool int_slist_push_back(IntSList *list, int value);
bool int_slist_pop_front(IntSList *list, int *out);
bool int_slist_pop_back(IntSList *list, int *out);
bool int_slist_get(const IntSList *list, size_t index, int *out);
bool int_slist_insert(IntSList *list, size_t index, int value);
bool int_slist_erase(IntSList *list, size_t index, int *out);
long long int_slist_sum(const IntSList *list);
bool int_slist_validate(const IntSList *list);

#endif
