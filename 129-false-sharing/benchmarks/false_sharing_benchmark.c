#include "false_sharing.h"
#include <stdio.h>
#include <stdint.h>
#include <time.h>
enum{THREADS=8};static const uint64_t ITER=UINT64_C(2000000);static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
static void run(size_t stride,const char*name){FsCounterArray*a=fs_counter_array_create(THREADS,stride,64);if(!a)return;struct timespec x,y;timespec_get(&x,TIME_UTC);bool ok=fs_parallel_increment(a,THREADS,ITER);timespec_get(&y,TIME_UTC);uint64_t sum=0,v=0;for(size_t i=0;i<THREADS;++i){fs_counter_load(a,i,&v);sum+=v;}printf("%s stride=%zu bytes=%zu seconds=%.6f ok=%d sum=%llu\n",name,fs_counter_stride(a),fs_storage_bytes(a),e(x,y),ok,(unsigned long long)sum);fs_counter_array_free(a);}
int main(void){run(sizeof(uint64_t),"packed");run(64,"line-separated");return 0;}
