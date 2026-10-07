#include "ds_serialization.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum{N=100000};
int main(void){
    assert(ds_crc32("123456789",9)==UINT32_C(0xcbf43926));
    DsRecord*r=malloc((size_t)N*sizeof(*r));assert(r);
    for(size_t i=0;i<N;++i){
        r[i].id=UINT64_C(1000000000)+i*17U;
        r[i].value=(i&1U)?-(int64_t)(i*31U+7U):(int64_t)(i*29U+3U);
        r[i].flags=(uint32_t)(i^0xa5a5U);
    }
    size_t size=0;assert(ds_serialized_size(N,&size)&&size==32U+(size_t)N*20U);
    unsigned char*blob=malloc(size);unsigned char*blob2=malloc(size);assert(blob&&blob2);
    size_t written=0;assert(ds_serialize(r,N,blob,size,&written)&&written==size&&ds_validate_blob(blob,size));
    size_t count=0;DsRecord*d=ds_deserialize(blob,size,&count);assert(d&&count==N);
    for(size_t i=0;i<N;++i)assert(d[i].id==r[i].id&&d[i].value==r[i].value&&d[i].flags==r[i].flags);
    size_t written2=0;assert(ds_serialize(d,count,blob2,size,&written2)&&written2==size&&memcmp(blob,blob2,size)==0);

    unsigned char saved=blob[size-1U];blob[size-1U]^=1U;assert(!ds_validate_blob(blob,size)&&!ds_deserialize(blob,size,&count));blob[size-1U]=saved;
    assert(!ds_validate_blob(blob,size-1U));
    saved=blob[0];blob[0]='X';assert(!ds_validate_blob(blob,size));blob[0]=saved;
    saved=blob[5];blob[5]=2U;assert(!ds_validate_blob(blob,size));blob[5]=saved;
    saved=blob[19];blob[19]=21U;assert(!ds_validate_blob(blob,size));blob[19]=saved;
    unsigned char*extra=malloc(size+1U);assert(extra);memcpy(extra,blob,size);extra[size]=0;assert(!ds_validate_blob(extra,size+1U));

    printf("Serialization tests passed; records=%zu bytes=%zu\n",count,size);
    free(extra);free(d);free(blob2);free(blob);free(r);return 0;
}
