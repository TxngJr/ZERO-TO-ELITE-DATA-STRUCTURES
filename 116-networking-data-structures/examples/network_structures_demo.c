#include "network_structures.h"
#include <stdio.h>
int main(void){RouteTrie*t=rt_create();FlowTable*f=flow_create(8);if(!t||!f)return 1;rt_add(t,0,0,1);rt_add(t,UINT32_C(0x0a000000),8,2);uint32_t hop=0;uint8_t len=0;bool found=false;rt_lookup(t,UINT32_C(0x0a010203),&hop,&len,&found);FlowKey k={UINT32_C(0x0a000001),UINT32_C(0x08080808),1234,53,17};bool ins=false;flow_put(f,k,99,&ins);printf("route_found=%d hop=%u prefix=%u flow_inserted=%d\n",found,hop,len,ins);flow_free(f);rt_free(t);return 0;}
