#include "cow_int_vector.h"
#include <stdio.h>
int main(void){int a[]={10,20,30};CowIntVector*x=cow_vector_from_array(a,3),*y=cow_vector_clone(x);if(!x||!y)return 1;printf("shared=%zu\n",cow_vector_share_count(x));if(!cow_vector_set(y,1,99))return 2;int xv=0,yv=0;cow_vector_get(x,1,&xv);cow_vector_get(y,1,&yv);printf("after write x=%d y=%d refs(x)=%zu refs(y)=%zu\n",xv,yv,cow_vector_share_count(x),cow_vector_share_count(y));cow_vector_free(x);cow_vector_free(y);return 0;}
