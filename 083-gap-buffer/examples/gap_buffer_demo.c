#include "gap_buffer.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){GapBuffer*b=gap_buffer_create(8);assert(b);const char*a="hello world";assert(gap_buffer_insert(b,0,(const uint8_t*)a,strlen(a)));const char*x="beautiful ";assert(gap_buffer_insert(b,6,(const uint8_t*)x,strlen(x)));uint8_t out[64];assert(gap_buffer_copy(b,out,sizeof out));fwrite(out,1,gap_buffer_size(b),stdout);putchar('\n');gap_buffer_free(b);return 0;}
