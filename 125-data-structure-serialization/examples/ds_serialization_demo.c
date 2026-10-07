#include "ds_serialization.h"
#include <stdio.h>
#include <stdlib.h>
int main(void){DsRecord r[2]={{1,-7,3},{2,99,5}};size_t size=0;if(!ds_serialized_size(2,&size))return 1;unsigned char*b=malloc(size);if(!b)return 2;size_t n=0;if(!ds_serialize(r,2,b,size,&n))return 3;size_t count=0;DsRecord*d=ds_deserialize(b,n,&count);if(!d)return 4;printf("bytes=%zu records=%zu second=%lld\n",n,count,(long long)d[1].value);free(d);free(b);return 0;}
