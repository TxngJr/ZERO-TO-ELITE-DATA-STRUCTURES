#include "compiler_structures.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum { NAMES=10000, VALUES=10000, EDGES=50000 };
static uint64_t state=UINT64_C(0x55aa1234deadbeef);
static uint64_t rng(void){
    state^=state<<13;
    state^=state>>7;
    state^=state<<17;
    return state;
}

int main(void){
    CompilerTables *t=ct_create(20000U,20000U,128U);
    assert(t&&ct_validate(t));

    NameId a=0U,b=0U;
    assert(ct_intern(t,"alpha",&a));
    assert(ct_intern(t,"alpha",&b));
    assert(a==b);

    CtSymbolInfo root_x={0};
    assert(ct_declare(t,"x",CT_SYMBOL_VARIABLE,1,&root_x));
    assert(!ct_declare(t,"x",CT_SYMBOL_VARIABLE,2,&root_x));

    assert(ct_enter_scope(t));
    CtSymbolInfo inner_x={0};
    assert(ct_declare(t,"x",CT_SYMBOL_VARIABLE,2,&inner_x));
    CtSymbolInfo got={0};
    bool found=false;
    assert(ct_lookup(t,"x",&got,&found)&&found&&got.type_tag==2);
    assert(ct_leave_scope(t));
    assert(ct_lookup(t,"x",&got,&found)&&found&&got.type_tag==1);

    for(size_t i=0;i<NAMES;++i){
        char name[32];
        snprintf(name,sizeof(name),"sym_%zu",i);
        CtSymbolInfo info={0};
        assert(ct_declare(t,name,CT_SYMBOL_VARIABLE,(int)(i%97U),&info));
    }
    assert(ct_validate(t));

    for(size_t q=0;q<20000U;++q){
        size_t i=(size_t)(rng()%NAMES);
        char name[32];
        snprintf(name,sizeof(name),"sym_%zu",i);
        assert(ct_lookup(t,name,&got,&found)&&found);
        assert(got.type_tag==(int)(i%97U));
    }

    UseDefGraph *g=udg_create(VALUES,EDGES);
    size_t *counts=calloc(VALUES,sizeof(*counts));
    assert(g&&counts);

    for(size_t i=0;i<EDGES;++i){
        size_t def=(size_t)(rng()%VALUES);
        size_t user=(size_t)(rng()%VALUES);
        assert(udg_add_use(g,def,user));
        ++counts[def];
    }

    assert(udg_edge_count(g)==EDGES&&udg_validate(g));
    for(size_t i=0;i<VALUES;++i)
        assert(udg_use_count(g,i)==counts[i]);

    size_t sample_def=(size_t)(rng()%VALUES);
    size_t cap=counts[sample_def];
    size_t *users=cap?malloc(cap*sizeof(*users)):NULL;
    size_t user_count=0U;
    assert(udg_collect_users(g,sample_def,users,cap,&user_count));
    assert(user_count==counts[sample_def]);

    puts("Compiler structures tests passed");
    free(users);
    free(counts);
    udg_free(g);
    ct_free(t);
    return 0;
}
