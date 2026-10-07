#include "knowledge_graph.h"
#include <assert.h>
#include <stdio.h>
enum{TERMS=10000,TRIPLES=100000};
int main(void){
    KnowledgeGraph*g=kg_create(TERMS+10,TRIPLES+10);assert(g);
    KgTermId ids[TERMS];char text[64];
    for(size_t i=0;i<TERMS;++i){
        snprintf(text,sizeof(text),"term-%05zu",i);
        bool inserted=false;assert(kg_intern(g,text,&ids[i],&inserted)&&inserted);
    }
    bool inserted=true;KgTermId again=0;
    assert(kg_intern(g,"term-00123",&again,&inserted)&&!inserted&&again==ids[123]);

    for(size_t i=0;i<TRIPLES;++i){
        size_t s=i/100U,p=i%100U;
        size_t o=2000U+((s*131U+p*17U)%8000U);
        KgTriple t={ids[s],ids[1000U+p],ids[o]};
        bool fresh=false;assert(kg_add_triple(g,t,&fresh)&&fresh);
    }
    KgTriple dup={ids[0],ids[1000],ids[2000]};
    assert(kg_add_triple(g,dup,&inserted)&&!inserted);
    assert(kg_build_indexes(g)&&kg_validate(g));

    KgTriple out[200];size_t count=0;
    assert(kg_query(g,(KgPattern){ids[7],0,0},out,200,&count)&&count==100U);
    assert(kg_query(g,(KgPattern){0,ids[1007],0},out,200,&count)&&count==1000U);
    size_t expected_object=0U;
    for(size_t s=0;s<1000U;++s)for(size_t p=0;p<100U;++p)
        if(2000U+((s*131U+p*17U)%8000U)==4321U)++expected_object;
    assert(kg_query(g,(KgPattern){0,0,ids[4321]},out,200,&count)&&count==expected_object);
    size_t exact_o=2000U+((321U*131U+21U*17U)%8000U);
    KgTriple exact={ids[321],ids[1021],ids[exact_o]};
    assert(kg_query(g,(KgPattern){exact.subject,exact.predicate,exact.object},out,200,&count)&&count==1U);
    assert(out[0].subject==exact.subject&&out[0].predicate==exact.predicate&&out[0].object==exact.object);

    KgTriple extra={ids[5],ids[1005],ids[9999]};
    assert(kg_add_triple(g,extra,&inserted)&&inserted&&!kg_indexes_fresh(g));
    assert(!kg_query(g,(KgPattern){ids[5],0,0},out,200,&count));
    assert(kg_build_indexes(g)&&kg_validate(g));
    assert(kg_query(g,(KgPattern){ids[5],0,0},out,200,&count)&&count==101U);

    printf("Knowledge graph tests passed; terms=%zu triples=%zu\n",kg_term_count(g),kg_triple_count(g));
    kg_free(g);return 0;
}
