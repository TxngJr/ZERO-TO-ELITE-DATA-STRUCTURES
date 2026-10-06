#include "lock_free_stack.h"
#include <stdio.h>
int main(void){LockFreeStack*s=lfs_create(8);if(!s)return 1;bool x=false,p=false;int v=0;lfs_push(s,10,&x);lfs_push(s,20,&x);lfs_pop(s,&v,&p);printf("popped=%d lock_free_atomics=%d\n",v,lfs_platform_lock_free(s));lfs_free(s);return p?0:2;}
