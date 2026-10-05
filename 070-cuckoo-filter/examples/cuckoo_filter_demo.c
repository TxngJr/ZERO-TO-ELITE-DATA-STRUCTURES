#include "byte_cuckoo_filter.h"
#include <assert.h>
#include <stdio.h>

int main(void){
    ByteCuckooFilter *f=byte_cuckoo_filter_create(64);
    assert(f);
    const uint8_t k[]={1,2,3};
    assert(byte_cuckoo_filter_add(f,k,sizeof k));
    bool maybe=false;
    assert(byte_cuckoo_filter_maybe_contains(f,k,sizeof k,&maybe));
    printf("maybe=%d size=%zu\n",maybe?1:0,byte_cuckoo_filter_size(f));
    byte_cuckoo_filter_free(f);
    return 0;
}
