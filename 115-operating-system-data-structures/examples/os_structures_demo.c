#include "os_structures.h"
#include <stdio.h>
int main(void){OsScheduler*os=os_create(16);if(!os)return 1;OsPid a,b,r;bool did=false;if(!os_spawn(os,1,&a)||!os_spawn(os,0,&b))return 2;if(!os_dispatch(os,&r,&did)||!did)return 3;printf("dispatched=%llu ready0=%zu\n",(unsigned long long)r,os_ready_count(os,0));if(!os_yield(os,r))return 4;os_free(os);return 0;}
