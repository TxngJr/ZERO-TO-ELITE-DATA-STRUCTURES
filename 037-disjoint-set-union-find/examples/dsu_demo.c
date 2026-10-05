#include "int_dsu.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntDSU *dsu = int_dsu_create(8);
    assert(dsu != NULL);

    bool merged = false;

    assert(int_dsu_union(dsu, 0, 1, &merged) && merged);
    assert(int_dsu_union(dsu, 1, 2, &merged) && merged);
    assert(int_dsu_union(dsu, 4, 5, &merged) && merged);
    assert(int_dsu_union(dsu, 2, 5, &merged) && merged);

    size_t size = 0;
    assert(int_dsu_component_size(dsu, 0, &size));

    printf("components=%zu size(component(0))=%zu\n",
           int_dsu_component_count(dsu),
           size);

    assert(int_dsu_validate(dsu));
    int_dsu_free(dsu);
    return 0;
}
