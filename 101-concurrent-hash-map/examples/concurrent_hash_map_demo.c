#include "concurrent_hash_map.h"
#include <stdio.h>
int main(void){ConcurrentHashMap*m=chm_create(4);if(!m)return 1;bool ins=false,f=false;int v=0;chm_put(m,42,99,&ins);chm_get(m,42,&v,&f);printf("inserted=%d found=%d value=%d\n",ins,f,v);chm_free(m);return f?0:2;}
