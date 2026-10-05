#include "int_bitset.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntBitset *set=int_bitset_create(70);
    assert(set!=NULL);

    assert(int_bitset_set(set,0));
    assert(int_bitset_set(set,63));
    assert(int_bitset_set(set,64));
    assert(int_bitset_set(set,69));

    size_t next=0;
    assert(int_bitset_find_next_set(set,1,&next));

    printf("count=%zu next_from_1=%zu\n",
           int_bitset_count(set),next);

    assert(int_bitset_validate(set));
    int_bitset_free(set);
    return 0;
}
