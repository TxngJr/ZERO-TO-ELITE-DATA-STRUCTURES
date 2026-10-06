#include "functional_queue.h"
#include <stdio.h>
int main(void){FQueueArena*a=fq_arena_create();if(!a)return 1;FunctionalQueue q0=fq_empty(),q1,q2,q3;if(!fq_enqueue(a,q0,10,&q1)||!fq_enqueue(a,q1,20,&q2))return 2;int v=0;if(!fq_dequeue(a,q2,&v,&q3))return 3;printf("dequeued=%d old_size=%zu new_size=%zu\n",v,fq_size(q2),fq_size(q3));fq_arena_free(a);return 0;}
