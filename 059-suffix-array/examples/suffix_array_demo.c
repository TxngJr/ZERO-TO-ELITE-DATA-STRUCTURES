#include "byte_suffix_array.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    const uint8_t *text=(const uint8_t *)"banana";
    ByteSuffixArray *array=byte_suffix_array_create(text,6);

    assert(array!=NULL);

    for(size_t r=0;r<6;++r) {
        size_t start=0,lcp=0;

        assert(byte_suffix_array_at(array,r,&start));
        assert(byte_suffix_array_lcp_at(array,r,&lcp));

        printf("rank=%zu start=%zu lcp=%zu\n",r,start,lcp);
    }

    const uint8_t *pattern=(const uint8_t *)"ana";
    size_t count=0;

    assert(byte_suffix_array_count(
        array,pattern,3,&count
    ));

    printf("ana occurrences=%zu\n",count);

    assert(byte_suffix_array_validate(array));
    byte_suffix_array_free(array);
    return 0;
}
