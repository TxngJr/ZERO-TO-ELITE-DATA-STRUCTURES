#include "compiler_structures.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define NO_INDEX SIZE_MAX

typedef struct {
    NameId name_id;
    CtSymbolKind kind;
    int type_tag;
    size_t scope_depth;
    size_t previous_binding;
} SymbolEntry;

struct CompilerTables {
    size_t name_capacity;
    size_t symbol_capacity;
    size_t scope_capacity;
    size_t name_count;
    size_t symbol_count;
    size_t scope_count;
    size_t hash_capacity;
    char **names;
    size_t *hash_slots;
    size_t *current_binding;
    SymbolEntry *symbols;
    size_t *scope_starts;
};

static uint64_t hash_string(const char *s){
    uint64_t h=UINT64_C(1469598103934665603);
    while(*s){
        h^=(unsigned char)*s++;
        h*=UINT64_C(1099511628211);
    }
    return h;
}

static bool next_pow2_at_least(size_t value,size_t *out){
    size_t p=1U;
    while(p<value){
        if(p>SIZE_MAX/2U)return false;
        p*=2U;
    }
    *out=p;
    return true;
}

CompilerTables *ct_create(size_t name_capacity,size_t symbol_capacity,
                          size_t scope_capacity){
    if(name_capacity==0U||symbol_capacity==0U||scope_capacity==0U||
       name_capacity>UINT32_MAX)return NULL;

    size_t desired=0U;
    if(name_capacity>SIZE_MAX/2U)return NULL;
    desired=name_capacity*2U;
    size_t hash_capacity=0U;
    if(!next_pow2_at_least(desired,&hash_capacity))return NULL;

    CompilerTables *t=calloc(1,sizeof(*t));
    if(!t)return NULL;

    t->names=calloc(name_capacity,sizeof(*t->names));
    t->hash_slots=malloc(hash_capacity*sizeof(*t->hash_slots));
    t->current_binding=malloc(name_capacity*sizeof(*t->current_binding));
    t->symbols=malloc(symbol_capacity*sizeof(*t->symbols));
    t->scope_starts=malloc(scope_capacity*sizeof(*t->scope_starts));
    if(!t->names||!t->hash_slots||!t->current_binding||
       !t->symbols||!t->scope_starts){
        ct_free(t);
        return NULL;
    }

    for(size_t i=0;i<hash_capacity;++i)t->hash_slots[i]=NO_INDEX;
    for(size_t i=0;i<name_capacity;++i)t->current_binding[i]=NO_INDEX;

    t->name_capacity=name_capacity;
    t->symbol_capacity=symbol_capacity;
    t->scope_capacity=scope_capacity;
    t->hash_capacity=hash_capacity;
    t->scope_count=1U;
    t->scope_starts[0]=0U;
    return t;
}

void ct_free(CompilerTables *t){
    if(!t)return;
    if(t->names){
        for(size_t i=0;i<t->name_count;++i)free(t->names[i]);
    }
    free(t->names);
    free(t->hash_slots);
    free(t->current_binding);
    free(t->symbols);
    free(t->scope_starts);
    free(t);
}

static bool find_name(const CompilerTables *t,const char *name,
                      size_t *out_slot,NameId *out_id,bool *out_found){
    if(!t||!name||!out_slot||!out_found)return false;
    size_t mask=t->hash_capacity-1U;
    size_t slot=(size_t)hash_string(name)&mask;

    for(size_t probe=0;probe<t->hash_capacity;++probe){
        size_t entry=t->hash_slots[slot];
        if(entry==NO_INDEX){
            *out_slot=slot;
            *out_found=false;
            return true;
        }
        if(entry<t->name_count&&strcmp(t->names[entry],name)==0){
            *out_slot=slot;
            if(out_id)*out_id=(NameId)entry;
            *out_found=true;
            return true;
        }
        slot=(slot+1U)&mask;
    }
    return false;
}

bool ct_intern(CompilerTables *t,const char *name,NameId *out_id){
    if(!t||!name||!out_id||name[0]=='\0')return false;
    size_t slot=0U;
    NameId id=0U;
    bool found=false;
    if(!find_name(t,name,&slot,&id,&found))return false;
    if(found){
        *out_id=id;
        return true;
    }
    if(t->name_count>=t->name_capacity)return false;

    size_t len=strlen(name);
    if(len==SIZE_MAX)return false;
    char *copy=malloc(len+1U);
    if(!copy)return false;
    memcpy(copy,name,len+1U);

    size_t index=t->name_count++;
    t->names[index]=copy;
    t->hash_slots[slot]=index;
    t->current_binding[index]=NO_INDEX;
    *out_id=(NameId)index;
    return true;
}

const char *ct_name(const CompilerTables *t,NameId id){
    return t&&id<t->name_count?t->names[id]:NULL;
}

bool ct_enter_scope(CompilerTables *t){
    if(!t||t->scope_count>=t->scope_capacity)return false;
    t->scope_starts[t->scope_count++]=t->symbol_count;
    return true;
}

bool ct_leave_scope(CompilerTables *t){
    if(!t||t->scope_count<=1U)return false;
    size_t start=t->scope_starts[t->scope_count-1U];
    while(t->symbol_count>start){
        size_t idx=t->symbol_count-1U;
        SymbolEntry *s=&t->symbols[idx];
        if(s->name_id>=t->name_count)return false;
        t->current_binding[s->name_id]=s->previous_binding;
        --t->symbol_count;
    }
    --t->scope_count;
    return true;
}

size_t ct_scope_depth(const CompilerTables *t){
    return t&&t->scope_count>0U?t->scope_count-1U:0U;
}

bool ct_declare(CompilerTables *t,const char *name,CtSymbolKind kind,
                int type_tag,CtSymbolInfo *out_info){
    if(!t||!out_info||
       (kind!=CT_SYMBOL_VARIABLE&&kind!=CT_SYMBOL_FUNCTION&&kind!=CT_SYMBOL_TYPE)||
       t->symbol_count>=t->symbol_capacity)return false;

    NameId id=0U;
    if(!ct_intern(t,name,&id))return false;

    size_t previous=t->current_binding[id];
    size_t depth=ct_scope_depth(t);
    if(previous!=NO_INDEX&&previous<t->symbol_count&&
       t->symbols[previous].scope_depth==depth)return false;

    size_t idx=t->symbol_count++;
    t->symbols[idx]=(SymbolEntry){id,kind,type_tag,depth,previous};
    t->current_binding[id]=idx;
    *out_info=(CtSymbolInfo){id,kind,type_tag,depth};
    return true;
}

bool ct_lookup(const CompilerTables *t,const char *name,
               CtSymbolInfo *out_info,bool *out_found){
    if(!t||!name||!out_info||!out_found)return false;
    size_t slot=0U;
    NameId id=0U;
    bool found_name=false;
    if(!find_name(t,name,&slot,&id,&found_name))return false;
    if(!found_name){
        *out_found=false;
        return true;
    }

    size_t idx=t->current_binding[id];
    if(idx==NO_INDEX){
        *out_found=false;
        return true;
    }
    if(idx>=t->symbol_count)return false;

    const SymbolEntry *s=&t->symbols[idx];
    *out_info=(CtSymbolInfo){s->name_id,s->kind,s->type_tag,s->scope_depth};
    *out_found=true;
    return true;
}

size_t ct_name_count(const CompilerTables *t){return t?t->name_count:0U;}
size_t ct_active_symbol_count(const CompilerTables *t){return t?t->symbol_count:0U;}

bool ct_validate(const CompilerTables *t){
    if(!t||!t->names||!t->hash_slots||!t->current_binding||
       !t->symbols||!t->scope_starts||t->scope_count==0U||
       t->scope_count>t->scope_capacity||t->name_count>t->name_capacity||
       t->symbol_count>t->symbol_capacity)return false;

    size_t *expected=malloc(t->name_capacity*sizeof(*expected));
    if(!expected)return false;
    for(size_t i=0;i<t->name_capacity;++i)expected[i]=NO_INDEX;

    bool ok=true;
    for(size_t i=0;i<t->name_count&&ok;++i){
        if(!t->names[i]||t->names[i][0]=='\0'){ok=false;break;}
        size_t slot=0U;
        NameId id=0U;
        bool found=false;
        if(!find_name(t,t->names[i],&slot,&id,&found)||!found||id!=i){
            ok=false;break;
        }
    }

    for(size_t i=0;i<t->symbol_count&&ok;++i){
        const SymbolEntry *s=&t->symbols[i];
        if(s->name_id>=t->name_count||s->scope_depth>=t->scope_count||
           s->previous_binding!=expected[s->name_id]){
            ok=false;break;
        }
        expected[s->name_id]=i;
    }

    for(size_t i=0;i<t->name_count&&ok;++i)
        if(t->current_binding[i]!=expected[i])ok=false;

    if(ok&&t->scope_starts[0]!=0U)ok=false;
    for(size_t i=1U;i<t->scope_count&&ok;++i)
        if(t->scope_starts[i]<t->scope_starts[i-1U]||
           t->scope_starts[i]>t->symbol_count)ok=false;

    free(expected);
    return ok;
}

typedef struct {
    size_t user;
    size_t next;
} UseEdge;

struct UseDefGraph {
    size_t value_count;
    size_t edge_capacity;
    size_t edge_count;
    size_t *heads;
    size_t *counts;
    UseEdge *edges;
};

UseDefGraph *udg_create(size_t value_count,size_t edge_capacity){
    if(value_count==0U||value_count>SIZE_MAX/sizeof(size_t)||
       edge_capacity>SIZE_MAX/sizeof(UseEdge))return NULL;

    UseDefGraph *g=calloc(1,sizeof(*g));
    if(!g)return NULL;
    g->heads=malloc(value_count*sizeof(*g->heads));
    g->counts=calloc(value_count,sizeof(*g->counts));
    g->edges=edge_capacity?malloc(edge_capacity*sizeof(*g->edges)):NULL;
    if(!g->heads||!g->counts||(edge_capacity&&!g->edges)){
        udg_free(g);
        return NULL;
    }

    for(size_t i=0;i<value_count;++i)g->heads[i]=NO_INDEX;
    g->value_count=value_count;
    g->edge_capacity=edge_capacity;
    return g;
}

void udg_free(UseDefGraph *g){
    if(!g)return;
    free(g->heads);
    free(g->counts);
    free(g->edges);
    free(g);
}

bool udg_add_use(UseDefGraph *g,size_t definition,size_t user){
    if(!g||definition>=g->value_count||user>=g->value_count||
       g->edge_count>=g->edge_capacity||g->counts[definition]==SIZE_MAX)
        return false;

    size_t idx=g->edge_count++;
    g->edges[idx]=(UseEdge){user,g->heads[definition]};
    g->heads[definition]=idx;
    ++g->counts[definition];
    return true;
}

size_t udg_use_count(const UseDefGraph *g,size_t definition){
    return g&&definition<g->value_count?g->counts[definition]:0U;
}

bool udg_collect_users(const UseDefGraph *g,size_t definition,
                       size_t *out,size_t out_capacity,size_t *out_count){
    if(!g||definition>=g->value_count||!out_count)return false;
    size_t produced=0U;
    size_t edge=g->heads[definition];
    while(edge!=NO_INDEX){
        if(edge>=g->edge_count||produced>=out_capacity)return false;
        if(out)out[produced]=g->edges[edge].user;
        ++produced;
        edge=g->edges[edge].next;
    }
    *out_count=produced;
    return true;
}

size_t udg_edge_count(const UseDefGraph *g){return g?g->edge_count:0U;}

bool udg_validate(const UseDefGraph *g){
    if(!g||!g->heads||!g->counts||
       (g->edge_capacity&&!g->edges)||g->edge_count>g->edge_capacity)
        return false;

    size_t total=0U;
    for(size_t d=0;d<g->value_count;++d){
        size_t count=0U;
        size_t edge=g->heads[d];
        while(edge!=NO_INDEX){
            if(edge>=g->edge_count||count>g->edge_count)return false;
            if(g->edges[edge].user>=g->value_count)return false;
            if(g->edges[edge].next!=NO_INDEX&&g->edges[edge].next>=edge)
                return false;
            ++count;
            edge=g->edges[edge].next;
        }
        if(count!=g->counts[d]||total>SIZE_MAX-count)return false;
        total+=count;
    }
    return total==g->edge_count;
}
