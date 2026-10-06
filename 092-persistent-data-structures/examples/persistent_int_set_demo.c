#include "persistent_int_set.h"
#include <stdio.h>
int main(void){PSetArena*a=pset_arena_create();if(!a)return 1;const PSetNode*v0=NULL,*v1=NULL,*v2=NULL;bool c=false;if(!pset_insert(a,v0,42,&v1,&c)||!pset_insert(a,v1,7,&v2,&c))return 2;printf("v0 has 42=%d, v1 has 7=%d, v2 size=%zu\n",pset_contains(v0,42),pset_contains(v1,7),pset_size(v2));pset_arena_free(a);return 0;}
