#include "ds_serialization.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
enum{N=1000000};static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){DsRecord*r=malloc((size_t)N*sizeof(*r));if(!r)return 1;for(size_t i=0;i<N;++i)r[i]=(DsRecord){i*13U,(int64_t)i*7-(int64_t)N,(uint32_t)i};size_t size=0;if(!ds_serialized_size(N,&size))return 2;unsigned char*b=malloc(size);if(!b)return 3;struct timespec a,c,d;timespec_get(&a,TIME_UTC);size_t written=0;if(!ds_serialize(r,N,b,size,&written))return 4;timespec_get(&c,TIME_UTC);size_t count=0;DsRecord*out=ds_deserialize(b,written,&count);if(!out||count!=N)return 5;timespec_get(&d,TIME_UTC);printf("records=%d bytes=%zu serialize_seconds=%.6f deserialize_seconds=%.6f\n",N,written,e(a,c),e(c,d));free(out);free(b);free(r);return 0;}
