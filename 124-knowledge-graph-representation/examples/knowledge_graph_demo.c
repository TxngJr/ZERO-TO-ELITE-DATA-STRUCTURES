#include "knowledge_graph.h"
#include <stdio.h>
int main(void){KnowledgeGraph*g=kg_create(16,16);if(!g)return 1;KgTermId alice,knows,bob;bool ins;if(!kg_intern(g,"alice",&alice,&ins)||!kg_intern(g,"knows",&knows,&ins)||!kg_intern(g,"bob",&bob,&ins))return 2;if(!kg_add_triple(g,(KgTriple){alice,knows,bob},&ins)||!kg_build_indexes(g))return 3;KgTriple out[4];size_t n=0;if(!kg_query(g,(KgPattern){alice,0,0},out,4,&n))return 4;printf("matches=%zu object=%s\n",n,kg_term(g,out[0].object));kg_free(g);return 0;}
