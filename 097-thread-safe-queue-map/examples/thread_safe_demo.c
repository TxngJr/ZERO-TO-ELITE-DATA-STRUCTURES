#include "thread_safe_structures.h"
#include <stdio.h>
int main(void){ThreadSafeQueue*q=tsq_create(4);ThreadSafeIntMap*m=tsm_create(8);if(!q||!m)return 1;bool x=false,p=false,f=false;int v=0;if(!tsq_try_push(q,7,&x)||!x)return 2;if(!tsq_try_pop(q,&v,&p)||!p)return 3;if(!tsm_put(m,42,99,&x)||!x)return 4;if(!tsm_get(m,42,&v,&f)||!f)return 5;printf("queue/map demo value=%d\n",v);tsq_free(q);tsm_free(m);return 0;}
