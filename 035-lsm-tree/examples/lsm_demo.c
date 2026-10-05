#include "int_lsm_tree.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntLSMTree *tree = int_lsm_tree_create(4);
    assert(tree != NULL);

    for (int key = 1; key <= 12; ++key) {
        assert(int_lsm_tree_put(tree, key, key * 100));
    }

    assert(int_lsm_tree_delete(tree, 3));
    assert(int_lsm_tree_put(tree, 5, 5555));
    assert(int_lsm_tree_flush(tree));
    assert(int_lsm_tree_validate(tree));

    printf(
        "visible=%zu runs=%zu mem=%zu\n",
        int_lsm_tree_size(tree),
        int_lsm_tree_run_count(tree),
        int_lsm_tree_memtable_size(tree)
    );

    assert(int_lsm_tree_compact(tree));
    assert(int_lsm_tree_validate(tree));

    int value = 0;
    assert(int_lsm_tree_get(tree, 5, &value));

    printf("key 5 -> %d, runs after compaction=%zu\n",
           value,
           int_lsm_tree_run_count(tree));

    int_lsm_tree_free(tree);
    return 0;
}
