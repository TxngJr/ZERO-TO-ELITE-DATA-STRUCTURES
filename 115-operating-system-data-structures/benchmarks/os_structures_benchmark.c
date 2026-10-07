#include "os_structures.h"
#include <stdio.h>
#include <time.h>
enum{PROCS=40000,STEPS=500000};static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){OsScheduler*os=os_create(PROCS);if(!os)return 1;OsPid pid;for(int i=0;i<PROCS;++i)if(!os_spawn(os,(uint8_t)(i%OS_PRIORITY_LEVELS),&pid))return 2;struct timespec a,b;timespec_get(&a,TIME_UTC);for(int i=0;i<STEPS;++i){bool did=false;if(!os_dispatch(os,&pid,&did)||!did||!os_yield(os,pid))return 3;}timespec_get(&b,TIME_UTC);double s=e(a,b);printf("dispatch_yield=%d seconds=%.6f ops_per_sec=%.0f\n",STEPS,s,(double)STEPS/s);os_free(os);return 0;}
