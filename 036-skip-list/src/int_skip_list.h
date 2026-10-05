#ifndef INT_SKIP_LIST_H
#define INT_SKIP_LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntSkipList IntSkipList;

IntSkipList *int_skip_list_create(size_t max_level);
void int_skip_list_free(IntSkipList *list);

size_t int_skip_list_size(const IntSkipList *list);
size_t int_skip_list_current_level(const IntSkipList *list);
size_t int_skip_list_max_level(const IntSkipList *list);

bool int_skip_list_contains(const IntSkipList *list, int key);
bool int_skip_list_insert(IntSkipList *list, int key);
bool int_skip_list_remove(IntSkipList *list, int key);

bool int_skip_list_range(
    const IntSkipList *list,
    int low,
    int high,
    int *output,
    size_t capacity,
    size_t *out_written
);

bool int_skip_list_validate(const IntSkipList *list);

#endif
