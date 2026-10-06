#include "u64_compressed_seq.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}int main(void){const size_t n=1000000;uint64_t*v=malloc(n*sizeof*v);if(!v)return 1;uint64_t x=0;for(size_t i=0;i<n;++i){x+=1+(i%101==0?10000:1);v[i]=x;}struct timespec a,b;timespec_get(&a,TIME_UTC);U64CompressedSeq*s=u64_compressed_seq_create(v,n,128);timespec_get(&b,TIME_UTC);if(!s)return 1;volatile uint64_t sum=0;struct timespec c,d;timespec_get(&c,TIME_UTC);for(size_t i=0;i<n;i+=101){uint64_t z=0;if(!u64_compressed_seq_get(s,i,&z))return 1;sum+=z;}timespec_get(&d,TIME_UTC);printf("n=%zu raw=%zu delta=%zu blocks=%zu build=%.9f reads=%.9f checksum=%llu\n",n,n*sizeof(uint64_t),u64_compressed_seq_encoded_bytes(s),u64_compressed_seq_block_count(s),e(a,b),e(c,d),(unsigned long long)sum);u64_compressed_seq_free(s);free(v);return 0;}
