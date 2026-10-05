#include "int_clist.h"
#include "int_dlist.h"
#include "int_slist.h"

#include <assert.h>
#include <stdio.h>

int main(void){
    IntSList *s=int_slist_create();
    IntDList *d=int_dlist_create();
    IntCList *c=int_clist_create();
    assert(s&&d&&c);

    for(int i=1;i<=3;++i){
        assert(int_slist_push_back(s,i));
        assert(int_dlist_push_back(d,i));
        assert(int_clist_push_back(c,i));
    }

    printf("sizes: singly=%zu doubly=%zu circular=%zu\n",
           int_slist_size(s),int_dlist_size(d),int_clist_size(c));

    assert(int_clist_rotate_left(c));
    int first=0;
    assert(int_clist_get(c,0,&first));
    printf("circular head after rotate=%d\n",first);

    assert(int_slist_validate(s));
    assert(int_dlist_validate(d));
    assert(int_clist_validate(c));

    int_slist_free(s);
    int_dlist_free(d);
    int_clist_free(c);
    return 0;
}
