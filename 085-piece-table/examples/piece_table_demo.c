#include "piece_table.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){const char*text="hello world";PieceTable*t=piece_table_create((const uint8_t*)text,strlen(text));assert(t);const char*x="big ";assert(piece_table_insert(t,6,(const uint8_t*)x,strlen(x)));assert(piece_table_erase(t,0,1));uint8_t out[64];assert(piece_table_copy(t,out,sizeof out));fwrite(out,1,piece_table_size(t),stdout);putchar('\n');printf("pieces=%zu add=%zu\n",piece_table_piece_count(t),piece_table_add_buffer_size(t));piece_table_free(t);return 0;}
