#include "piece_table.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}int main(void){PieceTable*t=piece_table_create(NULL,0);if(!t)return 1;const uint8_t x='x';const size_t n=100000;struct timespec a,b;timespec_get(&a,TIME_UTC);for(size_t i=0;i<n;++i)if(!piece_table_insert(t,piece_table_size(t),&x,1))return 1;timespec_get(&b,TIME_UTC);printf("size=%zu pieces=%zu add=%zu seconds=%.9f\n",piece_table_size(t),piece_table_piece_count(t),piece_table_add_buffer_size(t),e(a,b));piece_table_free(t);return 0;}
