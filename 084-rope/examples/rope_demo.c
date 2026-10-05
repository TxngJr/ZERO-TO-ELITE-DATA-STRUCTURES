#include "byte_rope.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){ByteRope*r=byte_rope_create(42);assert(r);const char*a="hello world";assert(byte_rope_insert(r,0,(const uint8_t*)a,strlen(a)));const char*b="big ";assert(byte_rope_insert(r,6,(const uint8_t*)b,strlen(b)));uint8_t out[64];assert(byte_rope_copy(r,out,sizeof out));fwrite(out,1,byte_rope_size(r),stdout);putchar('\n');byte_rope_free(r);return 0;}
