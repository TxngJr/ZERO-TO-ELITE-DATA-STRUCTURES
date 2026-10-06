#include "inverted_index.h"
#include <assert.h>
#include <stdio.h>
int main(void){InvertedIndex*x=inverted_index_create(16);assert(x);inverted_index_add_document(x,1,"red fox jumps");inverted_index_add_document(x,2,"blue fox sleeps");printf("fox df=%zu\n",inverted_index_doc_freq(x,"fox"));uint64_t out[4];size_t n=inverted_index_and(x,"fox","red",out,4);printf("fox AND red hits=%zu\n",n);inverted_index_free(x);return 0;}
