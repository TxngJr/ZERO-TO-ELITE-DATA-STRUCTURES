#ifndef KNOWLEDGE_GRAPH_H
#define KNOWLEDGE_GRAPH_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef uint32_t KgTermId;
typedef struct KnowledgeGraph KnowledgeGraph;
typedef struct {
    KgTermId subject;
    KgTermId predicate;
    KgTermId object;
} KgTriple;
typedef struct {
    KgTermId subject;
    KgTermId predicate;
    KgTermId object;
} KgPattern;
KnowledgeGraph *kg_create(size_t term_capacity,size_t triple_capacity);
void kg_free(KnowledgeGraph *graph);
bool kg_intern(KnowledgeGraph *graph,const char *text,KgTermId *out_id,bool *out_inserted);
const char *kg_term(const KnowledgeGraph *graph,KgTermId id);
bool kg_add_triple(KnowledgeGraph *graph,KgTriple triple,bool *out_inserted);
bool kg_build_indexes(KnowledgeGraph *graph);
bool kg_query(const KnowledgeGraph *graph,KgPattern pattern,KgTriple *out,size_t out_capacity,size_t *out_count);
size_t kg_term_count(const KnowledgeGraph *graph);
size_t kg_triple_count(const KnowledgeGraph *graph);
bool kg_indexes_fresh(const KnowledgeGraph *graph);
bool kg_validate(const KnowledgeGraph *graph);
#endif
