#include "wait_free_spsc_ring.h"
#include <stdio.h>
int main(void){WaitFreeSpscRing*r=wfr_create(8);if(!r)return 1;bool x=false,p=false;int v=0;wfr_try_push(r,42,&x);wfr_try_pop(r,&v,&p);printf("value=%d wait_free_atomic_assumption=%d usable=%zu\n",v,wfr_platform_lock_free(r),wfr_usable_capacity(r));wfr_free(r);return p?0:2;}
