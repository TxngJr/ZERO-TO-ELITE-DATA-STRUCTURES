#include "compiler_structures.h"
#include <stdio.h>

int main(void){
    CompilerTables *t=ct_create(64U,64U,16U);
    if(!t)return 1;

    CtSymbolInfo info={0};
    if(!ct_declare(t,"value",CT_SYMBOL_VARIABLE,1,&info))return 2;
    if(!ct_enter_scope(t))return 3;
    if(!ct_declare(t,"value",CT_SYMBOL_VARIABLE,2,&info))return 4;

    bool found=false;
    if(!ct_lookup(t,"value",&info,&found)||!found)return 5;
    printf("name=%s depth=%zu type=%d\n",
           ct_name(t,info.name_id),info.scope_depth,info.type_tag);

    ct_free(t);
    return 0;
}
