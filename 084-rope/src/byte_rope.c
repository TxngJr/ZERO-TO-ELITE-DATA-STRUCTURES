#include "byte_rope.h"
#include <stdint.h>
#include <stdlib.h>
typedef struct RopeNode{struct RopeNode*left,*right;uint64_t priority;size_t total;uint8_t byte;}RopeNode;
struct ByteRope{RopeNode*root;uint64_t rng;};
static size_t total(const RopeNode*n){return n?n->total:0;}
static void pull(RopeNode*n){if(n)n->total=1+total(n->left)+total(n->right);}
static uint64_t next_rng(ByteRope*r){uint64_t x=r->rng;x^=x>>12;x^=x<<25;x^=x>>27;r->rng=x;return x*UINT64_C(2685821657736338717);}
static RopeNode*node_new(ByteRope*r,uint8_t b){RopeNode*n=calloc(1,sizeof*n);if(!n)return NULL;n->priority=next_rng(r);n->total=1;n->byte=b;return n;}
static RopeNode*merge(RopeNode*a,RopeNode*b){if(!a)return b;if(!b)return a;if(a->priority>=b->priority){a->right=merge(a->right,b);pull(a);return a;}b->left=merge(a,b->left);pull(b);return b;}
static void split(RopeNode*root,size_t left_count,RopeNode**a,RopeNode**b){if(!root){*a=NULL;*b=NULL;return;}const size_t l=total(root->left);if(left_count<=l){split(root->left,left_count,a,&root->left);pull(root);*b=root;}else{split(root->right,left_count-l-1,&root->right,b);pull(root);*a=root;}}
static void free_tree(RopeNode*n){if(!n)return;free_tree(n->left);free_tree(n->right);free(n);}
ByteRope*byte_rope_create(uint64_t seed){ByteRope*r=calloc(1,sizeof*r);if(!r)return NULL;r->rng=seed?seed:UINT64_C(0x9e3779b97f4a7c15);return r;}
void byte_rope_free(ByteRope*r){if(!r)return;free_tree(r->root);free(r);}
size_t byte_rope_size(const ByteRope*r){return r?total(r->root):0;}
bool byte_rope_insert(ByteRope*r,size_t pos,const uint8_t*bytes,size_t len){if(!r||(len>0&&!bytes)||pos>byte_rope_size(r))return false;if(len==0)return true;if(len>SIZE_MAX-byte_rope_size(r))return false;const uint64_t old_rng=r->rng;RopeNode*piece=NULL;for(size_t i=0;i<len;++i){RopeNode*n=node_new(r,bytes[i]);if(!n){free_tree(piece);r->rng=old_rng;return false;}piece=merge(piece,n);}RopeNode*a=NULL,*b=NULL;split(r->root,pos,&a,&b);r->root=merge(merge(a,piece),b);return true;}
bool byte_rope_erase(ByteRope*r,size_t pos,size_t len){if(!r)return false;const size_t n=byte_rope_size(r);if(pos>n||len>n-pos)return false;if(len==0)return true;RopeNode*a=NULL,*rest=NULL,*mid=NULL,*b=NULL;split(r->root,pos,&a,&rest);split(rest,len,&mid,&b);free_tree(mid);r->root=merge(a,b);return true;}
bool byte_rope_get(const ByteRope*r,size_t i,uint8_t*out){if(!r||!out||i>=byte_rope_size(r))return false;RopeNode*n=r->root;while(n){size_t l=total(n->left);if(i<l)n=n->left;else if(i==l){*out=n->byte;return true;}else{i-=l+1;n=n->right;}}return false;}
static void copy_rec(const RopeNode*n,uint8_t*out,size_t*index){if(!n)return;copy_rec(n->left,out,index);out[(*index)++]=n->byte;copy_rec(n->right,out,index);}
bool byte_rope_copy(const ByteRope*r,uint8_t*out,size_t cap){if(!r)return false;const size_t n=byte_rope_size(r);if(n>0&&!out)return false;if(cap<n)return false;size_t i=0;copy_rec(r->root,out,&i);return i==n;}
typedef struct{bool ok;size_t total;}Check;
static Check check(const RopeNode*n){if(!n)return(Check){true,0};Check l=check(n->left),r=check(n->right);if(!l.ok||!r.ok)return(Check){false,0};if(n->left&&n->left->priority>n->priority)return(Check){false,0};if(n->right&&n->right->priority>n->priority)return(Check){false,0};if(l.total>SIZE_MAX-1)return(Check){false,0};if(r.total>SIZE_MAX-1-l.total)return(Check){false,0};size_t t=l.total+1+r.total;if(n->total!=t)return(Check){false,0};return(Check){true,t};}
bool byte_rope_validate(const ByteRope*r){if(!r)return false;Check c=check(r->root);return c.ok&&c.total==byte_rope_size(r);}
