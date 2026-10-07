#ifndef COMPILER_STRUCTURES_H
#define COMPILER_STRUCTURES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint32_t NameId;

typedef enum {
    CT_SYMBOL_VARIABLE=1,
    CT_SYMBOL_FUNCTION=2,
    CT_SYMBOL_TYPE=3
} CtSymbolKind;

typedef struct {
    NameId name_id;
    CtSymbolKind kind;
    int type_tag;
    size_t scope_depth;
} CtSymbolInfo;

typedef struct CompilerTables CompilerTables;

CompilerTables *ct_create(size_t name_capacity,size_t symbol_capacity,
                          size_t scope_capacity);
void ct_free(CompilerTables *tables);

bool ct_intern(CompilerTables *tables,const char *name,NameId *out_id);
const char *ct_name(const CompilerTables *tables,NameId id);

bool ct_enter_scope(CompilerTables *tables);
bool ct_leave_scope(CompilerTables *tables);
size_t ct_scope_depth(const CompilerTables *tables);

bool ct_declare(CompilerTables *tables,const char *name,CtSymbolKind kind,
                int type_tag,CtSymbolInfo *out_info);
bool ct_lookup(const CompilerTables *tables,const char *name,
               CtSymbolInfo *out_info,bool *out_found);

size_t ct_name_count(const CompilerTables *tables);
size_t ct_active_symbol_count(const CompilerTables *tables);
bool ct_validate(const CompilerTables *tables);

typedef struct UseDefGraph UseDefGraph;

UseDefGraph *udg_create(size_t value_count,size_t edge_capacity);
void udg_free(UseDefGraph *graph);
bool udg_add_use(UseDefGraph *graph,size_t definition,size_t user);
size_t udg_use_count(const UseDefGraph *graph,size_t definition);
bool udg_collect_users(const UseDefGraph *graph,size_t definition,
                       size_t *out,size_t out_capacity,size_t *out_count);
size_t udg_edge_count(const UseDefGraph *graph);
bool udg_validate(const UseDefGraph *graph);

#endif
