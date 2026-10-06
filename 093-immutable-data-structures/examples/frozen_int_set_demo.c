#include "frozen_int_set.h"
#include <stdio.h>
int main(void){int v[]={3,1,3,7,9};FrozenIntSet*s=frozen_int_set_create(v,5);if(!s)return 1;printf("size=%zu contains7=%d contains8=%d\n",frozen_int_set_size(s),frozen_int_set_contains(s,7),frozen_int_set_contains(s,8));frozen_int_set_free(s);return 0;}
