#include "false_sharing.h"
#include <stdio.h>
#include <stdint.h>
int main(void){FsCounterArray*a=fs_counter_array_create(4,sizeof(uint64_t),64);if(!a)return 1;bool share=false;if(!fs_counters_share_line(a,0,1,&share))return 2;if(!fs_parallel_increment(a,4,10000))return 3;uint64_t v=0;fs_counter_load(a,0,&v);printf("stride=%zu line=%zu c0_c1_share=%d c0=%llu\n",fs_counter_stride(a),fs_line_size(a),share,(unsigned long long)v);fs_counter_array_free(a);return 0;}
