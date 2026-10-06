#include "concurrent_mpmc_ring.h"
#include <stdio.h>
int main(void){ConcurrentMpmcRing*r=cmr_create(8);if(!r)return 1;bool p=false,q=false;int v=0;cmr_try_push(r,42,&p);cmr_try_pop(r,&v,&q);printf("pushed=%d popped=%d value=%d lock_free_atomics=%d\n",p,q,v,cmr_platform_lock_free(r));cmr_free(r);return q?0:2;}
