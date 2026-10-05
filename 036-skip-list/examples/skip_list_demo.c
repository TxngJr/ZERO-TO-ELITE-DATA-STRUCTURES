#include "int_skip_list.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntSkipList *list = int_skip_list_create(24);
    assert(list != NULL);

    for (int key = 1; key <= 50; ++key) {
        assert(int_skip_list_insert(list, key));
    }

    assert(int_skip_list_validate(list));

    int values[16];
    size_t written = 0;

    assert(int_skip_list_range(
        list,
        20,
        30,
        values,
        16,
        &written
    ));

    printf(
        "size=%zu active_levels=%zu range:",
        int_skip_list_size(list),
        int_skip_list_current_level(list)
    );

    for (size_t i = 0; i < written; ++i) {
        printf(" %d", values[i]);
    }
    putchar('\n');

    int_skip_list_free(list);
    return 0;
}
