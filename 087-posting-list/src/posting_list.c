#include "posting_list.h"
#include <stdlib.h>
#include <string.h>
struct PostingList{uint32_t*docs;size_t size,capacity;};
PostingList*posting_list_create(void){return calloc(1,sizeof(PostingList));}
void posting_list_free(PostingList*l){if(!l)return;free(l->docs);free(l);}
size_t posting_list_size(const PostingList*l){return l?l->size:0;}
static size_t lower_bound_doc(const PostingList*l,uint32_t d){size_t lo=0,hi=l->size;while(lo<hi){size_t m=lo+(hi-lo)/2;if(l->docs[m]<d)lo=m+1;else hi=m;}return lo;}
static bool reserve_docs(PostingList*l,size_t need){if(need<=l->capacity)return true;size_t c=l->capacity?l->capacity*2:8;if(c<need)c=need;if(c>SIZE_MAX/sizeof(uint32_t))return false;uint32_t*p=realloc(l->docs,c*sizeof*p);if(!p)return false;l->docs=p;l->capacity=c;return true;}
bool posting_list_add(PostingList*l,uint32_t d){if(!l)return false;size_t i=lower_bound_doc(l,d);if(i<l->size&&l->docs[i]==d)return true;if(l->size==SIZE_MAX||!reserve_docs(l,l->size+1))return false;memmove(&l->docs[i+1],&l->docs[i],(l->size-i)*sizeof*l->docs);l->docs[i]=d;++l->size;return true;}
bool posting_list_contains(const PostingList*l,uint32_t d){if(!l)return false;size_t i=lower_bound_doc(l,d);return i<l->size&&l->docs[i]==d;}
bool posting_list_get(const PostingList*l,size_t i,uint32_t*out){if(!l||!out||i>=l->size)return false;*out=l->docs[i];return true;}
PostingList*posting_list_intersect(const PostingList*a,const PostingList*b){if(!a||!b)return NULL;PostingList*out=posting_list_create();if(!out)return NULL;size_t max=a->size<b->size?a->size:b->size;if(max&&!reserve_docs(out,max)){posting_list_free(out);return NULL;}size_t i=0,j=0;while(i<a->size&&j<b->size){if(a->docs[i]==b->docs[j]){out->docs[out->size++]=a->docs[i];++i;++j;}else if(a->docs[i]<b->docs[j])++i;else ++j;}return out;}
static size_t varint_size(uint32_t x){size_t n=1;while(x>=128U){x>>=7;++n;}return n;}
static bool write_varint(uint32_t x,uint8_t*out,size_t cap,size_t*pos){while(x>=128U){if(*pos>=cap)return false;out[(*pos)++]=(uint8_t)((x&0x7fU)|0x80U);x>>=7;}if(*pos>=cap)return false;out[(*pos)++]=(uint8_t)x;return true;}
bool posting_list_encode_gaps(const PostingList*l,uint8_t*out,size_t cap,size_t*written){if(!l||!written)return false;size_t need=0;uint32_t prev=0;for(size_t i=0;i<l->size;++i){uint32_t gap=i==0?l->docs[i]:l->docs[i]-prev;size_t s=varint_size(gap);if(SIZE_MAX-need<s)return false;need+=s;prev=l->docs[i];}*written=need;if(need==0)return true;if(!out||cap<need)return false;size_t pos=0;prev=0;for(size_t i=0;i<l->size;++i){uint32_t gap=i==0?l->docs[i]:l->docs[i]-prev;if(!write_varint(gap,out,cap,&pos))return false;prev=l->docs[i];}return pos==need;}
static bool read_varint(const uint8_t*data,size_t len,size_t*pos,uint32_t*out){uint32_t value=0;unsigned shift=0;for(unsigned i=0;i<5;++i){if(*pos>=len)return false;uint8_t byte=data[(*pos)++];if(i==4&&(byte&0xF0U)!=0)return false;value|=(uint32_t)(byte&0x7fU)<<shift;if((byte&0x80U)==0){*out=value;return true;}shift+=7;}return false;}
PostingList*posting_list_decode_gaps(const uint8_t*data,size_t len){if(len>0&&!data)return NULL;PostingList*l=posting_list_create();if(!l)return NULL;size_t pos=0;uint32_t prev=0;bool first=true;while(pos<len){uint32_t gap=0;if(!read_varint(data,len,&pos,&gap)){posting_list_free(l);return NULL;}uint32_t doc;if(first){doc=gap;first=false;}else{if(UINT32_MAX-prev<gap){posting_list_free(l);return NULL;}doc=prev+gap;if(doc<=prev){posting_list_free(l);return NULL;}}if(!posting_list_add(l,doc)){posting_list_free(l);return NULL;}prev=doc;}return l;}
bool posting_list_validate(const PostingList*l){if(!l||l->size>l->capacity||(l->capacity&&!l->docs))return false;for(size_t i=1;i<l->size;++i)if(l->docs[i-1]>=l->docs[i])return false;return true;}
