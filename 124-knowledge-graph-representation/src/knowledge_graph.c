#include "knowledge_graph.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *text;
    KgTermId id;
    bool used;
} TermSlot;

typedef struct {
    KgTriple triple;
    bool used;
} TripleSlot;

struct KnowledgeGraph {
    size_t term_capacity,triple_capacity;
    size_t term_count,triple_count;
    char **terms;
    KgTriple *triples;
    TermSlot *term_map;
    size_t term_map_capacity;
    TripleSlot *triple_set;
    size_t triple_set_capacity;
    size_t *spo,*pos,*osp;
    uint64_t triple_version,index_version;
};

static uint64_t mix64(uint64_t x){
    x^=x>>30U;x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27U;x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31U;return x;
}

static uint64_t text_hash(const char *s){
    uint64_t h=UINT64_C(1469598103934665603);
    while(*s){
        h^=(unsigned char)*s++;
        h*=UINT64_C(1099511628211);
    }
    return h;
}

static uint64_t triple_hash(KgTriple t){
    uint64_t x=((uint64_t)t.subject<<32U)|t.predicate;
    x^=mix64((uint64_t)t.object+UINT64_C(0x9e3779b97f4a7c15));
    return mix64(x);
}

static bool triple_equal(KgTriple a,KgTriple b){
    return a.subject==b.subject&&a.predicate==b.predicate&&a.object==b.object;
}

static bool map_capacity_for(size_t capacity,size_t *out){
    if(capacity>SIZE_MAX/2U)return false;
    size_t need=capacity*2U;
    size_t cap=8U;
    while(cap<need){
        if(cap>SIZE_MAX/2U)return false;
        cap*=2U;
    }
    *out=cap;
    return true;
}

KnowledgeGraph *kg_create(size_t term_capacity,size_t triple_capacity){
    if(term_capacity==0U||triple_capacity==0U||term_capacity>UINT32_MAX-1U||
       term_capacity>SIZE_MAX/sizeof(char*)||
       triple_capacity>SIZE_MAX/sizeof(KgTriple))return NULL;

    size_t tm=0U,ts=0U;
    if(!map_capacity_for(term_capacity,&tm)||
       !map_capacity_for(triple_capacity,&ts)||
       tm>SIZE_MAX/sizeof(TermSlot)||
       ts>SIZE_MAX/sizeof(TripleSlot))return NULL;

    KnowledgeGraph*g=calloc(1,sizeof(*g));
    if(!g)return NULL;
    g->terms=calloc(term_capacity,sizeof(*g->terms));
    g->triples=malloc(triple_capacity*sizeof(*g->triples));
    g->term_map=calloc(tm,sizeof(*g->term_map));
    g->triple_set=calloc(ts,sizeof(*g->triple_set));
    if(!g->terms||!g->triples||!g->term_map||!g->triple_set){
        kg_free(g);return NULL;
    }
    g->term_capacity=term_capacity;g->triple_capacity=triple_capacity;
    g->term_map_capacity=tm;g->triple_set_capacity=ts;
    g->triple_version=1U;
    return g;
}

void kg_free(KnowledgeGraph*g){
    if(!g)return;
    for(size_t i=0;i<g->term_count;++i)free(g->terms[i]);
    free(g->terms);free(g->triples);free(g->term_map);free(g->triple_set);
    free(g->spo);free(g->pos);free(g->osp);free(g);
}

bool kg_intern(KnowledgeGraph*g,const char*text,KgTermId*out_id,bool*out_inserted){
    if(!g||!text||!*text||!out_id)return false;
    size_t mask=g->term_map_capacity-1U;
    size_t slot=(size_t)mix64(text_hash(text))&mask;
    for(size_t probes=0;probes<g->term_map_capacity;++probes){
        TermSlot*e=&g->term_map[slot];
        if(!e->used){
            if(g->term_count>=g->term_capacity)return false;
            size_t len=strlen(text);
            if(len==SIZE_MAX)return false;
            char*copy=malloc(len+1U);
            if(!copy)return false;
            memcpy(copy,text,len+1U);
            KgTermId id=(KgTermId)(g->term_count+1U);
            g->terms[g->term_count++]=copy;
            e->text=copy;e->id=id;e->used=true;
            *out_id=id;
            if(out_inserted)*out_inserted=true;
            return true;
        }
        if(strcmp(e->text,text)==0){
            *out_id=e->id;
            if(out_inserted)*out_inserted=false;
            return true;
        }
        slot=(slot+1U)&mask;
    }
    return false;
}

const char *kg_term(const KnowledgeGraph*g,KgTermId id){
    if(!g||id==0U||id>g->term_count)return NULL;
    return g->terms[id-1U];
}

bool kg_add_triple(KnowledgeGraph*g,KgTriple t,bool*out_inserted){
    if(!g||t.subject==0U||t.predicate==0U||t.object==0U||
       t.subject>g->term_count||t.predicate>g->term_count||t.object>g->term_count)
        return false;

    size_t mask=g->triple_set_capacity-1U;
    size_t slot=(size_t)triple_hash(t)&mask;
    for(size_t probes=0;probes<g->triple_set_capacity;++probes){
        TripleSlot*e=&g->triple_set[slot];
        if(!e->used){
            if(g->triple_count>=g->triple_capacity)return false;
            g->triples[g->triple_count++]=t;
            e->triple=t;e->used=true;
            ++g->triple_version;
            if(g->triple_version==0U)++g->triple_version;
            if(out_inserted)*out_inserted=true;
            return true;
        }
        if(triple_equal(e->triple,t)){
            if(out_inserted)*out_inserted=false;
            return true;
        }
        slot=(slot+1U)&mask;
    }
    return false;
}

static const KgTriple *cmp_triples;
static int cmp_spo(const void*ap,const void*bp){
    size_t a=*(const size_t*)ap,b=*(const size_t*)bp;
    KgTriple x=cmp_triples[a],y=cmp_triples[b];
    if(x.subject!=y.subject)return x.subject<y.subject?-1:1;
    if(x.predicate!=y.predicate)return x.predicate<y.predicate?-1:1;
    if(x.object!=y.object)return x.object<y.object?-1:1;
    return 0;
}
static int cmp_pos(const void*ap,const void*bp){
    size_t a=*(const size_t*)ap,b=*(const size_t*)bp;
    KgTriple x=cmp_triples[a],y=cmp_triples[b];
    if(x.predicate!=y.predicate)return x.predicate<y.predicate?-1:1;
    if(x.object!=y.object)return x.object<y.object?-1:1;
    if(x.subject!=y.subject)return x.subject<y.subject?-1:1;
    return 0;
}
static int cmp_osp(const void*ap,const void*bp){
    size_t a=*(const size_t*)ap,b=*(const size_t*)bp;
    KgTriple x=cmp_triples[a],y=cmp_triples[b];
    if(x.object!=y.object)return x.object<y.object?-1:1;
    if(x.subject!=y.subject)return x.subject<y.subject?-1:1;
    if(x.predicate!=y.predicate)return x.predicate<y.predicate?-1:1;
    return 0;
}

bool kg_build_indexes(KnowledgeGraph*g){
    if(!g)return false;
    if(g->triple_count>SIZE_MAX/sizeof(size_t))return false;
    size_t*spo=g->triple_count?malloc(g->triple_count*sizeof(*spo)):NULL;
    size_t*pos=g->triple_count?malloc(g->triple_count*sizeof(*pos)):NULL;
    size_t*osp=g->triple_count?malloc(g->triple_count*sizeof(*osp)):NULL;
    if(g->triple_count&&(!spo||!pos||!osp)){
        free(spo);free(pos);free(osp);return false;
    }
    for(size_t i=0;i<g->triple_count;++i)spo[i]=pos[i]=osp[i]=i;
    cmp_triples=g->triples;
    qsort(spo,g->triple_count,sizeof(*spo),cmp_spo);
    qsort(pos,g->triple_count,sizeof(*pos),cmp_pos);
    qsort(osp,g->triple_count,sizeof(*osp),cmp_osp);
    cmp_triples=NULL;

    free(g->spo);free(g->pos);free(g->osp);
    g->spo=spo;g->pos=pos;g->osp=osp;
    g->index_version=g->triple_version;
    return true;
}

bool kg_indexes_fresh(const KnowledgeGraph*g){
    return g&&g->index_version!=0U&&g->index_version==g->triple_version;
}

static KgTermId first_key(const KgTriple*t,int order){
    return order==0?t->subject:(order==1?t->predicate:t->object);
}

static bool matches(KgTriple t,KgPattern p){
    return (p.subject==0U||p.subject==t.subject)&&
           (p.predicate==0U||p.predicate==t.predicate)&&
           (p.object==0U||p.object==t.object);
}

bool kg_query(const KnowledgeGraph*g,KgPattern p,KgTriple*out,size_t cap,size_t*out_count){
    if(!g||!out_count||!kg_indexes_fresh(g)||(cap&& !out))return false;
    if((p.subject&&p.subject>g->term_count)||
       (p.predicate&&p.predicate>g->term_count)||
       (p.object&&p.object>g->term_count))return false;

    const size_t*index=g->spo;
    KgTermId bound=0U;
    int order=0;
    if(p.subject){bound=p.subject;index=g->spo;order=0;}
    else if(p.predicate){bound=p.predicate;index=g->pos;order=1;}
    else if(p.object){bound=p.object;index=g->osp;order=2;}

    size_t begin=0U,end=g->triple_count;
    if(bound){
        size_t lo=0U,hi=g->triple_count;
        while(lo<hi){
            size_t mid=lo+(hi-lo)/2U;
            KgTermId key=first_key(&g->triples[index[mid]],order);
            if(key<bound)lo=mid+1U;else hi=mid;
        }
        begin=lo;hi=g->triple_count;
        while(lo<hi){
            size_t mid=lo+(hi-lo)/2U;
            KgTermId key=first_key(&g->triples[index[mid]],order);
            if(key<=bound)lo=mid+1U;else hi=mid;
        }
        end=lo;
    }

    size_t count=0U;
    for(size_t i=begin;i<end;++i){
        KgTriple t=g->triples[index[i]];
        if(matches(t,p)){
            if(count<cap)out[count]=t;
            ++count;
        }
    }
    *out_count=count;
    return true;
}

size_t kg_term_count(const KnowledgeGraph*g){return g?g->term_count:0U;}
size_t kg_triple_count(const KnowledgeGraph*g){return g?g->triple_count:0U;}

static bool validate_permutation(const KnowledgeGraph*g,const size_t*index,int order){
    if(g->triple_count&&!index)return false;
    unsigned char*seen=g->triple_count?calloc(g->triple_count,1U):NULL;
    if(g->triple_count&&!seen)return false;
    for(size_t i=0;i<g->triple_count;++i){
        size_t ti=index[i];
        if(ti>=g->triple_count||seen[ti]){free(seen);return false;}
        seen[ti]=1U;
        if(i){
            KgTriple a=g->triples[index[i-1U]],b=g->triples[ti];
            int cmp=0;
            if(order==0){
                if(a.subject!=b.subject)cmp=a.subject<b.subject?-1:1;
                else if(a.predicate!=b.predicate)cmp=a.predicate<b.predicate?-1:1;
                else if(a.object!=b.object)cmp=a.object<b.object?-1:1;
            }else if(order==1){
                if(a.predicate!=b.predicate)cmp=a.predicate<b.predicate?-1:1;
                else if(a.object!=b.object)cmp=a.object<b.object?-1:1;
                else if(a.subject!=b.subject)cmp=a.subject<b.subject?-1:1;
            }else{
                if(a.object!=b.object)cmp=a.object<b.object?-1:1;
                else if(a.subject!=b.subject)cmp=a.subject<b.subject?-1:1;
                else if(a.predicate!=b.predicate)cmp=a.predicate<b.predicate?-1:1;
            }
            if(cmp>0){free(seen);return false;}
        }
    }
    free(seen);return true;
}

bool kg_validate(const KnowledgeGraph*g){
    if(!g||!g->terms||!g->triples||!g->term_map||!g->triple_set||
       g->term_count>g->term_capacity||g->triple_count>g->triple_capacity||
       !kg_indexes_fresh(g))return false;
    for(size_t i=0;i<g->term_count;++i)
        if(!g->terms[i]||!*g->terms[i])return false;
    for(size_t i=0;i<g->triple_count;++i){
        KgTriple t=g->triples[i];
        if(t.subject==0U||t.predicate==0U||t.object==0U||
           t.subject>g->term_count||t.predicate>g->term_count||t.object>g->term_count)
            return false;
    }
    return validate_permutation(g,g->spo,0)&&
           validate_permutation(g,g->pos,1)&&
           validate_permutation(g,g->osp,2);
}
