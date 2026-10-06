#include "posting_list.h"
#include <assert.h>
#include <stdio.h>
int main(void){PostingList*l=posting_list_create();assert(l);posting_list_add(l,3);posting_list_add(l,10);posting_list_add(l,1000);size_t bytes=0;assert(posting_list_encode_gaps(l,NULL,0,&bytes));printf("docs=%zu encoded_bytes=%zu\n",posting_list_size(l),bytes);posting_list_free(l);return 0;}
