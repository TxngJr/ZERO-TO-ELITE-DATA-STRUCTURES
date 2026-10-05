#include "int_clist.h"
#include "int_dlist.h"
#include "int_slist.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void assert_slist(const IntSList *l,const int *ref,size_t n){
    assert(int_slist_validate(l));
    assert(int_slist_size(l)==n);
    for(size_t i=0;i<n;++i){int x=0;assert(int_slist_get(l,i,&x));assert(x==ref[i]);}
}

static void assert_dlist(const IntDList *l,const int *ref,size_t n){
    assert(int_dlist_validate(l));
    assert(int_dlist_size(l)==n);
    for(size_t i=0;i<n;++i){int x=0;assert(int_dlist_get(l,i,&x));assert(x==ref[i]);}
}

static uint32_t next_rng(uint32_t *s){*s=*s*1664525u+1013904223u;return *s;}

static void test_linear_lists_randomized(void){
    enum{MAX=256,STEPS=5000};
    IntSList *s=int_slist_create();
    IntDList *d=int_dlist_create();
    assert(s&&d);
    int ref[MAX];size_t n=0;uint32_t rng=0xBADC0DEu;

    for(int step=0;step<STEPS;++step){
        unsigned op=next_rng(&rng)%4U;
        if((op==0U||n==0)&&n<MAX){
            size_t index=next_rng(&rng)%(n+1);
            int value=(int)next_rng(&rng);
            assert(int_slist_insert(s,index,value));
            assert(int_dlist_insert(d,index,value));
            memmove(&ref[index+1],&ref[index],(n-index)*sizeof ref[0]);
            ref[index]=value;++n;
        }else if(op==1U&&n>0){
            size_t index=next_rng(&rng)%n;
            int a=0,b=0;
            assert(int_slist_erase(s,index,&a));
            assert(int_dlist_erase(d,index,&b));
            assert(a==ref[index]&&b==ref[index]);
            memmove(&ref[index],&ref[index+1],(n-index-1)*sizeof ref[0]);
            --n;
        }else if(op==2U&&n<MAX){
            int value=(int)next_rng(&rng);
            assert(int_slist_push_front(s,value));
            assert(int_dlist_push_front(d,value));
            memmove(&ref[1],&ref[0],n*sizeof ref[0]);
            ref[0]=value;++n;
        }else if(n>0){
            int a=0,b=0;
            assert(int_slist_pop_back(s,&a));
            assert(int_dlist_pop_back(d,&b));
            assert(a==ref[n-1]&&b==ref[n-1]);--n;
        }
        assert_slist(s,ref,n);
        assert_dlist(d,ref,n);
    }
    int_slist_free(s);int_dlist_free(d);
}

static void test_circular(void){
    IntCList *c=int_clist_create();assert(c);
    assert(int_clist_validate(c));
    assert(int_clist_push_back(c,1));
    assert(int_clist_push_back(c,2));
    assert(int_clist_push_back(c,3));
    assert(int_clist_validate(c));

    int x=0;
    assert(int_clist_get(c,0,&x)&&x==1);
    assert(int_clist_rotate_left(c));
    assert(int_clist_get(c,0,&x)&&x==2);
    assert(int_clist_rotate_left(c));
    assert(int_clist_get(c,0,&x)&&x==3);
    assert(int_clist_rotate_left(c));
    assert(int_clist_get(c,0,&x)&&x==1);

    assert(int_clist_pop_front(c,&x)&&x==1);
    assert(int_clist_pop_front(c,&x)&&x==2);
    assert(int_clist_pop_front(c,&x)&&x==3);
    assert(int_clist_size(c)==0);
    assert(int_clist_validate(c));
    int_clist_free(c);
}

int main(void){
    test_linear_lists_randomized();
    test_circular();
    puts("linked-list tests passed");
    return 0;
}
