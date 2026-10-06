#include "atomic_structures.h"
#include <stdio.h>
int main(void){AtomicBitset*s=abs_create(128);AtomicTaggedValue*v=atv_create(7);if(!s||!v)return 1;bool old=false;abs_test_and_set(s,42,&old);ATVSnapshot snap={0};bool swapped=false;atv_load(v,&snap);atv_compare_exchange(v,&snap,9,&swapped);printf("bit42 previous=%d tagged_swap=%d lock_free=%d\n",old,swapped,abs_platform_lock_free(s)&&atv_platform_lock_free(v));abs_free(s);atv_free(v);return 0;}
