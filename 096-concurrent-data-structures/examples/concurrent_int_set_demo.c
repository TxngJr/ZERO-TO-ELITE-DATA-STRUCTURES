#include "concurrent_int_set.h"
#include <stdio.h>
int main(void){ConcurrentIntSet*s=cis_create();if(!s)return 1;bool changed=false,has=false;if(!cis_insert(s,42,&changed)||!cis_contains(s,42,&has))return 2;printf("inserted=%d contains42=%d\n",changed,has);cis_free(s);return 0;}
