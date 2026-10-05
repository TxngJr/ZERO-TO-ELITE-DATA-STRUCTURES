#include "int_bit_vector.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    const uint8_t bits[]={0,1,0,1,1,0,1};

    IntBitVector *vector=
        int_bit_vector_create(bits,sizeof bits);

    assert(vector!=NULL);

    size_t rank=0;
    size_t selected=0;

    assert(int_bit_vector_rank1(vector,7,&rank));
    assert(int_bit_vector_select1(vector,2,&selected));

    printf("ones=%zu rank1(7)=%zu select1(2)=%zu\n",
           int_bit_vector_ones(vector),rank,selected);

    assert(int_bit_vector_validate(vector));
    int_bit_vector_free(vector);
    return 0;
}
