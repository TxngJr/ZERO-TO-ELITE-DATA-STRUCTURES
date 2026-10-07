#include "mpt.h"
#include <stdio.h>
#include <string.h>
int main(void){Mpt*t=mpt_create();if(!t)return 1;unsigned char a[MPT_KEY_BYTES]={0},b[MPT_KEY_BYTES]={0};a[31]=1;b[31]=2;bool ins;if(!mpt_put(t,a,100,&ins)||!mpt_put(t,b,200,&ins))return 2;unsigned char root[32];if(!mpt_root(t,root))return 3;printf("size=%zu nodes=%zu root=%02x%02x%02x%02x...\n",mpt_size(t),mpt_node_count(t),root[0],root[1],root[2],root[3]);mpt_free(t);return 0;}
