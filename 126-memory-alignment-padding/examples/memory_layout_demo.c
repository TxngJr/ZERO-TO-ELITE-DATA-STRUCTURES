#include "memory_layout.h"
#include <stdio.h>
int main(void){MaField f[3]={{1,1},{8,8},{4,4}};MaLayout l;if(!ma_compute_layout(f,3,&l))return 1;printf("offsets=%zu,%zu,%zu size=%zu alignment=%zu padding=%zu\n",l.offsets[0],l.offsets[1],l.offsets[2],l.struct_size,l.struct_alignment,l.padding_bytes);return 0;}
