#include "graph_db.h"
#include <stdlib.h>
#include <string.h>

typedef struct { uint64_t id; uint32_t label; int64_t property; } Node;
typedef struct { size_t src,dst; uint32_t type; int64_t weight; } Edge;
typedef struct { uint64_t id; size_t node_index; bool used; } NodeMapEntry;
typedef struct { uint32_t label; uint64_t id; size_t node_index; } LabelEntry;

struct GraphDb {
    size_t node_capacity,edge_capacity,node_count,edge_count;
    Node *nodes;
    Edge *edges;
    NodeMapEntry *map;
    size_t map_capacity;
    uint64_t version,index_version;
    size_t *out_offsets,*out_edges,*in_offsets,*in_edges;
    LabelEntry *labels;
};

static uint64_t mix64(uint64_t x){
    x^=x>>30U;x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27U;x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31U;return x;
}

static bool map_capacity_for(size_t nodes,size_t*out){
    if(nodes>SIZE_MAX/2U)return false;
    size_t need=nodes*2U,cap=8U;
    while(cap<need){if(cap>SIZE_MAX/2U)return false;cap*=2U;}
    *out=cap;return true;
}

GraphDb *gdb_create(size_t node_capacity,size_t edge_capacity){
    if(node_capacity==0U||edge_capacity==0U||
       node_capacity>SIZE_MAX/sizeof(Node)||
       edge_capacity>SIZE_MAX/sizeof(Edge))return NULL;
    size_t map_capacity=0U;
    if(!map_capacity_for(node_capacity,&map_capacity)||
       map_capacity>SIZE_MAX/sizeof(NodeMapEntry))return NULL;

    GraphDb*g=calloc(1,sizeof(*g));if(!g)return NULL;
    g->nodes=malloc(node_capacity*sizeof(*g->nodes));
    g->edges=malloc(edge_capacity*sizeof(*g->edges));
    g->map=calloc(map_capacity,sizeof(*g->map));
    if(!g->nodes||!g->edges||!g->map){gdb_free(g);return NULL;}
    g->node_capacity=node_capacity;g->edge_capacity=edge_capacity;
    g->map_capacity=map_capacity;g->version=1U;
    return g;
}

void gdb_free(GraphDb*g){
    if(!g)return;
    free(g->nodes);free(g->edges);free(g->map);
    free(g->out_offsets);free(g->out_edges);
    free(g->in_offsets);free(g->in_edges);free(g->labels);
    free(g);
}

static bool find_node_index(const GraphDb*g,uint64_t id,size_t*out,bool*found){
    size_t mask=g->map_capacity-1U,index=(size_t)mix64(id)&mask;
    for(size_t probes=0;probes<g->map_capacity;++probes){
        const NodeMapEntry*e=&g->map[index];
        if(!e->used){*found=false;return true;}
        if(e->id==id){*out=e->node_index;*found=true;return true;}
        index=(index+1U)&mask;
    }
    return false;
}

bool gdb_add_node(GraphDb*g,uint64_t id,uint32_t label,int64_t property){
    if(!g||g->node_count>=g->node_capacity)return false;
    size_t existing=0U;bool found=false;
    if(!find_node_index(g,id,&existing,&found)||found)return false;

    size_t index=g->node_count;
    g->nodes[index]=(Node){id,label,property};

    size_t mask=g->map_capacity-1U,slot=(size_t)mix64(id)&mask;
    while(g->map[slot].used)slot=(slot+1U)&mask;
    g->map[slot]=(NodeMapEntry){id,index,true};

    ++g->node_count;++g->version;if(g->version==0U)++g->version;
    return true;
}

bool gdb_add_edge(GraphDb*g,uint64_t src_id,uint64_t dst_id,uint32_t type,int64_t weight){
    if(!g||g->edge_count>=g->edge_capacity)return false;
    size_t src=0U,dst=0U;bool sf=false,df=false;
    if(!find_node_index(g,src_id,&src,&sf)||!sf||
       !find_node_index(g,dst_id,&dst,&df)||!df)return false;
    g->edges[g->edge_count++]=(Edge){src,dst,type,weight};
    ++g->version;if(g->version==0U)++g->version;
    return true;
}

static int label_compare(const void*ap,const void*bp){
    const LabelEntry*a=ap,*b=bp;
    if(a->label<b->label)return -1;
    if(a->label>b->label)return 1;
    if(a->id<b->id)return -1;
    if(a->id>b->id)return 1;
    return 0;
}

bool gdb_build_indexes(GraphDb*g){
    if(!g)return false;
    if(g->node_count==SIZE_MAX)return false;
    size_t offsets_count=g->node_count+1U;
    if(offsets_count>SIZE_MAX/sizeof(size_t)||
       g->edge_count>SIZE_MAX/sizeof(size_t)||
       g->node_count>SIZE_MAX/sizeof(LabelEntry))return false;

    size_t*out_offsets=calloc(offsets_count,sizeof(*out_offsets));
    size_t*in_offsets=calloc(offsets_count,sizeof(*in_offsets));
    size_t*out_edges=g->edge_count?malloc(g->edge_count*sizeof(*out_edges)):NULL;
    size_t*in_edges=g->edge_count?malloc(g->edge_count*sizeof(*in_edges)):NULL;
    LabelEntry*labels=malloc(g->node_count*sizeof(*labels));
    size_t*out_cursor=malloc(g->node_count*sizeof(*out_cursor));
    size_t*in_cursor=malloc(g->node_count*sizeof(*in_cursor));
    if(!out_offsets||!in_offsets||!labels||!out_cursor||!in_cursor||
       (g->edge_count&&(!out_edges||!in_edges))){
        free(out_offsets);free(in_offsets);free(out_edges);free(in_edges);
        free(labels);free(out_cursor);free(in_cursor);return false;
    }

    for(size_t e=0;e<g->edge_count;++e){
        ++out_offsets[g->edges[e].src+1U];
        ++in_offsets[g->edges[e].dst+1U];
    }
    for(size_t n=0;n<g->node_count;++n){
        out_offsets[n+1U]+=out_offsets[n];
        in_offsets[n+1U]+=in_offsets[n];
        out_cursor[n]=out_offsets[n];
        in_cursor[n]=in_offsets[n];
        labels[n]=(LabelEntry){g->nodes[n].label,g->nodes[n].id,n};
    }
    for(size_t e=0;e<g->edge_count;++e){
        out_edges[out_cursor[g->edges[e].src]++]=e;
        in_edges[in_cursor[g->edges[e].dst]++]=e;
    }
    qsort(labels,g->node_count,sizeof(*labels),label_compare);
    free(out_cursor);free(in_cursor);

    free(g->out_offsets);free(g->out_edges);
    free(g->in_offsets);free(g->in_edges);free(g->labels);
    g->out_offsets=out_offsets;g->out_edges=out_edges;
    g->in_offsets=in_offsets;g->in_edges=in_edges;g->labels=labels;
    g->index_version=g->version;
    return true;
}

bool gdb_node(GraphDb*g,uint64_t id,uint32_t*out_label,int64_t*out_property){
    if(!g||!out_label||!out_property)return false;
    size_t index=0U;bool found=false;
    if(!find_node_index(g,id,&index,&found)||!found)return false;
    *out_label=g->nodes[index].label;*out_property=g->nodes[index].property;
    return true;
}

bool gdb_indexes_fresh(const GraphDb*g){
    return g&&g->index_version!=0U&&g->index_version==g->version;
}

static bool neighbors(const GraphDb*g,uint64_t id,uint32_t type,bool outgoing,
                      uint64_t*out,size_t cap,size_t*out_count){
    if(!g||!out_count||!gdb_indexes_fresh(g)||(cap&& !out))return false;
    size_t node=0U;bool found=false;
    if(!find_node_index(g,id,&node,&found)||!found)return false;
    const size_t*offsets=outgoing?g->out_offsets:g->in_offsets;
    const size_t*edge_ids=outgoing?g->out_edges:g->in_edges;
    size_t count=0U;
    for(size_t p=offsets[node];p<offsets[node+1U];++p){
        const Edge*e=&g->edges[edge_ids[p]];
        if(e->type!=type)continue;
        size_t other=outgoing?e->dst:e->src;
        if(count<cap)out[count]=g->nodes[other].id;
        ++count;
    }
    *out_count=count;return true;
}

bool gdb_out_neighbors(const GraphDb*g,uint64_t id,uint32_t type,uint64_t*out,size_t cap,size_t*out_count){
    return neighbors(g,id,type,true,out,cap,out_count);
}
bool gdb_in_neighbors(const GraphDb*g,uint64_t id,uint32_t type,uint64_t*out,size_t cap,size_t*out_count){
    return neighbors(g,id,type,false,out,cap,out_count);
}

bool gdb_nodes_with_label(const GraphDb*g,uint32_t label,uint64_t*out,size_t cap,size_t*out_count){
    if(!g||!out_count||!gdb_indexes_fresh(g)||(cap&& !out))return false;
    size_t lo=0U,hi=g->node_count;
    while(lo<hi){size_t mid=lo+(hi-lo)/2U;if(g->labels[mid].label<label)lo=mid+1U;else hi=mid;}
    size_t first=lo;hi=g->node_count;
    while(lo<hi){size_t mid=lo+(hi-lo)/2U;if(g->labels[mid].label<=label)lo=mid+1U;else hi=mid;}
    size_t last=lo,count=last-first;
    size_t written=count<cap?count:cap;
    for(size_t i=0;i<written;++i)out[i]=g->labels[first+i].id;
    *out_count=count;return true;
}

bool gdb_shortest_hops(const GraphDb*g,uint64_t source_id,uint64_t target_id,
                       uint32_t edge_type,size_t*out_hops,bool*out_found){
    if(!g||!out_hops||!out_found||!gdb_indexes_fresh(g))return false;
    size_t source=0U,target=0U;bool sf=false,tf=false;
    if(!find_node_index(g,source_id,&source,&sf)||!sf||
       !find_node_index(g,target_id,&target,&tf)||!tf)return false;
    if(source==target){*out_hops=0U;*out_found=true;return true;}

    unsigned char*seen=calloc(g->node_count,1U);
    size_t*queue=malloc(g->node_count*sizeof(*queue));
    size_t*depth=malloc(g->node_count*sizeof(*depth));
    if(!seen||!queue||!depth){free(seen);free(queue);free(depth);return false;}

    size_t head=0U,tail=0U;queue[tail]=source;depth[tail++]=0U;seen[source]=1U;
    bool found=false;size_t hops=0U;
    while(head<tail&&!found){
        size_t node=queue[head],d=depth[head];++head;
        for(size_t p=g->out_offsets[node];p<g->out_offsets[node+1U];++p){
            const Edge*e=&g->edges[g->out_edges[p]];
            if(e->type!=edge_type||seen[e->dst])continue;
            if(e->dst==target){found=true;hops=d+1U;break;}
            seen[e->dst]=1U;queue[tail]=e->dst;depth[tail]=d+1U;++tail;
        }
    }
    free(seen);free(queue);free(depth);
    *out_found=found;if(found)*out_hops=hops;return true;
}

size_t gdb_node_count(const GraphDb*g){return g?g->node_count:0U;}
size_t gdb_edge_count(const GraphDb*g){return g?g->edge_count:0U;}

bool gdb_validate(const GraphDb*g){
    if(!g||!g->nodes||!g->edges||!g->map||g->node_count>g->node_capacity||
       g->edge_count>g->edge_capacity||!gdb_indexes_fresh(g)||
       !g->out_offsets||!g->in_offsets||!g->labels)return false;
    if(g->out_offsets[0]!=0U||g->in_offsets[0]!=0U||
       g->out_offsets[g->node_count]!=g->edge_count||
       g->in_offsets[g->node_count]!=g->edge_count)return false;

    for(size_t n=0;n<g->node_count;++n){
        size_t idx=0U;bool found=false;
        if(!find_node_index(g,g->nodes[n].id,&idx,&found)||!found||idx!=n)return false;
        if(g->out_offsets[n]>g->out_offsets[n+1U]||
           g->in_offsets[n]>g->in_offsets[n+1U])return false;
    }

    unsigned char*seen_nodes=calloc(g->node_count,1U);
    unsigned char*seen_out=g->edge_count?calloc(g->edge_count,1U):NULL;
    unsigned char*seen_in=g->edge_count?calloc(g->edge_count,1U):NULL;
    if(!seen_nodes||(g->edge_count&&(!seen_out||!seen_in))){
        free(seen_nodes);free(seen_out);free(seen_in);return false;
    }

    for(size_t i=0;i<g->node_count;++i){
        const LabelEntry*l=&g->labels[i];
        if(l->node_index>=g->node_count||seen_nodes[l->node_index]||
           l->id!=g->nodes[l->node_index].id||
           l->label!=g->nodes[l->node_index].label){
            free(seen_nodes);free(seen_out);free(seen_in);return false;
        }
        if(i){
            const LabelEntry*p=&g->labels[i-1U];
            if(p->label>l->label||(p->label==l->label&&p->id>=l->id)){
                free(seen_nodes);free(seen_out);free(seen_in);return false;
            }
        }
        seen_nodes[l->node_index]=1U;
    }

    for(size_t n=0;n<g->node_count;++n){
        for(size_t p=g->out_offsets[n];p<g->out_offsets[n+1U];++p){
            size_t e=g->out_edges[p];
            if(e>=g->edge_count||seen_out[e]||g->edges[e].src!=n){
                free(seen_nodes);free(seen_out);free(seen_in);return false;
            }
            seen_out[e]=1U;
        }
        for(size_t p=g->in_offsets[n];p<g->in_offsets[n+1U];++p){
            size_t e=g->in_edges[p];
            if(e>=g->edge_count||seen_in[e]||g->edges[e].dst!=n){
                free(seen_nodes);free(seen_out);free(seen_in);return false;
            }
            seen_in[e]=1U;
        }
    }
    for(size_t e=0;e<g->edge_count;++e)
        if(!seen_out[e]||!seen_in[e]){
            free(seen_nodes);free(seen_out);free(seen_in);return false;
        }

    free(seen_nodes);free(seen_out);free(seen_in);return true;
}
