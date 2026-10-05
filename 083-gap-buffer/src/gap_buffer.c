#include "gap_buffer.h"
#include <stdlib.h>
#include <string.h>
struct GapBuffer{uint8_t*data;size_t capacity,gap_start,gap_end;};
static size_t logical_size(const GapBuffer*b){return b->capacity-(b->gap_end-b->gap_start);}
GapBuffer*gap_buffer_create(size_t g){if(g==0)g=16;if(g>SIZE_MAX/sizeof(uint8_t))return NULL;GapBuffer*b=calloc(1,sizeof*b);if(!b)return NULL;b->data=malloc(g);if(!b->data){free(b);return NULL;}b->capacity=g;b->gap_end=g;return b;}
void gap_buffer_free(GapBuffer*b){if(!b)return;free(b->data);free(b);}
size_t gap_buffer_size(const GapBuffer*b){return b?logical_size(b):0;}
size_t gap_buffer_capacity(const GapBuffer*b){return b?b->capacity:0;}
size_t gap_buffer_gap_position(const GapBuffer*b){return b?b->gap_start:0;}
size_t gap_buffer_gap_size(const GapBuffer*b){return b?b->gap_end-b->gap_start:0;}
static bool move_gap(GapBuffer*b,size_t pos){const size_t n=logical_size(b);if(pos>n)return false;if(pos<b->gap_start){const size_t d=b->gap_start-pos;memmove(b->data+b->gap_end-d,b->data+pos,d);b->gap_start-=d;b->gap_end-=d;}else if(pos>b->gap_start){const size_t d=pos-b->gap_start;memmove(b->data+b->gap_start,b->data+b->gap_end,d);b->gap_start+=d;b->gap_end+=d;}return true;}
static bool ensure_gap(GapBuffer*b,size_t need){size_t gap=b->gap_end-b->gap_start;if(gap>=need)return true;const size_t n=logical_size(b);if(need>SIZE_MAX-n)return false;const size_t required=n+need;size_t c=b->capacity?b->capacity:16;while(c<required){if(c>SIZE_MAX/2){c=required;break;}c*=2;}uint8_t*p=malloc(c);if(!p)return false;const size_t suffix=b->capacity-b->gap_end;memcpy(p,b->data,b->gap_start);const size_t new_gap_end=c-suffix;memcpy(p+new_gap_end,b->data+b->gap_end,suffix);free(b->data);b->data=p;b->capacity=c;b->gap_end=new_gap_end;return true;}
bool gap_buffer_insert(GapBuffer*b,size_t pos,const uint8_t*bytes,size_t len){if(!b||(len>0&&!bytes))return false;if(!move_gap(b,pos))return false;if(len==0)return true;if(!ensure_gap(b,len))return false;memcpy(b->data+b->gap_start,bytes,len);b->gap_start+=len;return true;}
bool gap_buffer_erase(GapBuffer*b,size_t pos,size_t len){if(!b)return false;const size_t n=logical_size(b);if(pos>n||len>n-pos)return false;if(!move_gap(b,pos))return false;b->gap_end+=len;return true;}
bool gap_buffer_get(const GapBuffer*b,size_t i,uint8_t*out){if(!b||!out||i>=logical_size(b))return false;*out=i<b->gap_start?b->data[i]:b->data[b->gap_end+(i-b->gap_start)];return true;}
bool gap_buffer_copy(const GapBuffer*b,uint8_t*out,size_t cap){if(!b)return false;const size_t n=logical_size(b);if(n>0&&!out)return false;if(cap<n)return false;memcpy(out,b->data,b->gap_start);const size_t suffix=b->capacity-b->gap_end;memcpy(out+b->gap_start,b->data+b->gap_end,suffix);return true;}
bool gap_buffer_validate(const GapBuffer*b){if(!b||!b->data||b->capacity==0||b->gap_start>b->gap_end||b->gap_end>b->capacity)return false;const size_t n=logical_size(b);if(b->gap_start>n)return false;return b->capacity-b->gap_end==n-b->gap_start;}
