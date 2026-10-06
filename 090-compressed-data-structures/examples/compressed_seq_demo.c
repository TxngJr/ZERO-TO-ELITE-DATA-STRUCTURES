#include "u64_compressed_seq.h"
#include <assert.h>
#include <stdio.h>
int main(void){uint64_t v[]={10,11,12,100,101,1000};U64CompressedSeq*s=u64_compressed_seq_create(v,6,3);assert(s);printf("values=%zu blocks=%zu delta_bytes=%zu\n",u64_compressed_seq_size(s),u64_compressed_seq_block_count(s),u64_compressed_seq_encoded_bytes(s));uint64_t x=0;assert(u64_compressed_seq_get(s,4,&x));printf("v[4]=%llu lower_bound(99)=%zu\n",(unsigned long long)x,u64_compressed_seq_lower_bound(s,99));u64_compressed_seq_free(s);return 0;}
