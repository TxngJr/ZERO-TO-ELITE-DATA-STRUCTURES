#include "inverted_index.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}int main(void){InvertedIndex*x=inverted_index_create(1024);if(!x)return 1;const size_t n=50000;struct timespec a,b;timespec_get(&a,TIME_UTC);for(uint64_t i=0;i<n;++i){char text[128];snprintf(text,sizeof text,"common group%llu item%llu",(unsigned long long)(i%100),(unsigned long long)i);if(!inverted_index_add_document(x,i,text))return 1;}timespec_get(&b,TIME_UTC);printf("docs=%zu terms=%zu common_df=%zu seconds=%.9f\n",n,inverted_index_term_count(x),inverted_index_doc_freq(x,"common"),e(a,b));inverted_index_free(x);return 0;}
