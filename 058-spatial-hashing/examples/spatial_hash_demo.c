#include "int_spatial_hash.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntSpatialHash *hash=int_spatial_hash_create(10);
    assert(hash!=NULL);

    const IntSpatialPoint points[]={
        {-1,-1,1},{0,0,2},{9,9,3},{10,10,4},{23,4,5}
    };

    for(size_t i=0;i<5;++i) {
        assert(int_spatial_hash_insert(hash,points[i]));
    }

    size_t count=0;

    assert(int_spatial_hash_query_count(
        hash,-5,15,-5,15,&count
    ));

    printf(
        "count=%zu occupied_cells=%zu\n",
        count,int_spatial_hash_cell_count(hash)
    );

    assert(int_spatial_hash_validate(hash));
    int_spatial_hash_free(hash);
    return 0;
}
