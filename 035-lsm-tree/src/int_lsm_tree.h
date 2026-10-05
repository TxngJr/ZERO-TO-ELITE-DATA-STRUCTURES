#ifndef INT_LSM_TREE_H
#define INT_LSM_TREE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntLSMTree IntLSMTree;

IntLSMTree *int_lsm_tree_create(size_t memtable_capacity);
void int_lsm_tree_free(IntLSMTree *tree);

size_t int_lsm_tree_size(const IntLSMTree *tree);
size_t int_lsm_tree_memtable_size(const IntLSMTree *tree);
size_t int_lsm_tree_run_count(const IntLSMTree *tree);

bool int_lsm_tree_put(IntLSMTree *tree, int key, int value);
bool int_lsm_tree_delete(IntLSMTree *tree, int key);
bool int_lsm_tree_get(const IntLSMTree *tree, int key, int *out_value);

bool int_lsm_tree_flush(IntLSMTree *tree);
bool int_lsm_tree_compact(IntLSMTree *tree);

bool int_lsm_tree_scan(
    const IntLSMTree *tree,
    int low,
    int high,
    int *keys,
    int *values,
    size_t capacity,
    size_t *out_written
);

bool int_lsm_tree_validate(const IntLSMTree *tree);

#endif
