#include "consistent_hash_ring.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint64_t token;
    uint64_t node_id;
    size_t replica;
} RingPoint;

struct ConsistentHashRing {
    RingPoint *points;
    size_t point_count;
    size_t point_capacity;
    uint64_t *nodes;
    size_t node_count;
    size_t node_capacity;
    size_t virtual_nodes;
};

static uint64_t mix64(uint64_t x){
    x^=x>>30;x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27;x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31;return x;
}

static uint64_t hash_bytes(const uint8_t *key,size_t length){
    uint64_t h=UINT64_C(1469598103934665603);
    for(size_t i=0;i<length;++i){h^=key[i];h*=UINT64_C(1099511628211);}
    return mix64(h^(uint64_t)length);
}

static uint64_t vnode_token(uint64_t node,size_t replica){
    return mix64(node^mix64((uint64_t)replica+UINT64_C(0x9e3779b97f4a7c15)));
}

static int cmp_point(const void *a,const void *b){
    const RingPoint *x=a,*y=b;
    if(x->token<y->token)return -1;if(x->token>y->token)return 1;
    if(x->node_id<y->node_id)return -1;if(x->node_id>y->node_id)return 1;
    return x->replica<y->replica?-1:(x->replica>y->replica?1:0);
}

static bool reserve_points(ConsistentHashRing *r,size_t need){
    if(need<=r->point_capacity)return true;
    size_t cap=r->point_capacity?r->point_capacity:16;
    while(cap<need){if(cap>SIZE_MAX/2){cap=need;break;}cap*=2;}
    if(cap>SIZE_MAX/sizeof *r->points)return false;
    RingPoint *p=realloc(r->points,cap*sizeof *p);if(!p)return false;
    r->points=p;r->point_capacity=cap;return true;
}

static bool reserve_nodes(ConsistentHashRing *r,size_t need){
    if(need<=r->node_capacity)return true;
    size_t cap=r->node_capacity?r->node_capacity:8;
    while(cap<need){if(cap>SIZE_MAX/2){cap=need;break;}cap*=2;}
    if(cap>SIZE_MAX/sizeof *r->nodes)return false;
    uint64_t *p=realloc(r->nodes,cap*sizeof *p);if(!p)return false;
    r->nodes=p;r->node_capacity=cap;return true;
}

ConsistentHashRing *consistent_hash_ring_create(size_t vnodes){
    if(vnodes==0)return NULL;
    ConsistentHashRing *r=calloc(1,sizeof *r);if(!r)return NULL;
    r->virtual_nodes=vnodes;return r;
}

void consistent_hash_ring_free(ConsistentHashRing *r){if(!r)return;free(r->points);free(r->nodes);free(r);}
size_t consistent_hash_ring_node_count(const ConsistentHashRing *r){return r?r->node_count:0;}
size_t consistent_hash_ring_point_count(const ConsistentHashRing *r){return r?r->point_count:0;}

static bool has_node(const ConsistentHashRing *r,uint64_t id){for(size_t i=0;i<r->node_count;++i)if(r->nodes[i]==id)return true;return false;}

bool consistent_hash_ring_add_node(ConsistentHashRing *r,uint64_t id){
    if(!r||has_node(r,id)||r->node_count==SIZE_MAX)return false;
    if(r->virtual_nodes>SIZE_MAX-r->point_count)return false;
    const size_t new_points=r->point_count+r->virtual_nodes;
    if(!reserve_points(r,new_points)||!reserve_nodes(r,r->node_count+1))return false;

    for(size_t replica=0;replica<r->virtual_nodes;++replica){
        r->points[r->point_count++]=(RingPoint){vnode_token(id,replica),id,replica};
    }
    r->nodes[r->node_count++]=id;
    qsort(r->points,r->point_count,sizeof *r->points,cmp_point);
    return true;
}

bool consistent_hash_ring_remove_node(ConsistentHashRing *r,uint64_t id){
    if(!r)return false;
    size_t ni=SIZE_MAX;for(size_t i=0;i<r->node_count;++i)if(r->nodes[i]==id){ni=i;break;}
    if(ni==SIZE_MAX)return false;
    size_t out=0;
    for(size_t i=0;i<r->point_count;++i)if(r->points[i].node_id!=id)r->points[out++]=r->points[i];
    r->point_count=out;
    r->nodes[ni]=r->nodes[r->node_count-1];--r->node_count;
    return true;
}

bool consistent_hash_ring_lookup(const ConsistentHashRing *r,const uint8_t *key,size_t length,uint64_t *out){
    if(!r||!out||(length>0&&!key)||r->point_count==0)return false;
    const uint64_t token=hash_bytes(key,length);
    size_t lo=0,hi=r->point_count;
    while(lo<hi){size_t mid=lo+(hi-lo)/2;if(r->points[mid].token<token)lo=mid+1;else hi=mid;}
    if(lo==r->point_count)lo=0;
    *out=r->points[lo].node_id;return true;
}

bool consistent_hash_ring_validate(const ConsistentHashRing *r){
    if(!r||r->virtual_nodes==0||r->node_count>r->node_capacity||r->point_count>r->point_capacity)return false;
    if(r->node_count>0&&r->nodes==NULL)return false;
    if(r->point_count>0&&r->points==NULL)return false;
    if(r->virtual_nodes!=0&&r->node_count>SIZE_MAX/r->virtual_nodes)return false;
    if(r->point_count!=r->node_count*r->virtual_nodes)return false;
    for(size_t i=1;i<r->point_count;++i)if(cmp_point(&r->points[i-1],&r->points[i])>0)return false;
    for(size_t i=0;i<r->node_count;++i){
        for(size_t j=i+1;j<r->node_count;++j)if(r->nodes[i]==r->nodes[j])return false;
        size_t count=0;for(size_t p=0;p<r->point_count;++p)if(r->points[p].node_id==r->nodes[i])++count;
        if(count!=r->virtual_nodes)return false;
    }
    return true;
}
