#include "db_index.h"
#include <stdio.h>

int main(void){
    DbRow rows[]={
        {30U,7U,300},
        {10U,7U,100},
        {20U,4U,200}
    };
    DbIndex *index=dbi_build(rows,3U);
    if(!index)return 1;

    DbRow out[4];
    size_t count=0U;
    if(!dbi_secondary_equal(index,7U,out,4U,&count))return 2;
    printf("secondary=7 count=%zu first_pk=%llu\n",
           count,(unsigned long long)out[0].primary_key);

    dbi_free(index);
    return 0;
}
