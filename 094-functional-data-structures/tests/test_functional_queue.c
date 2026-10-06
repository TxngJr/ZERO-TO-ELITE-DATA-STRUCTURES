#include "functional_queue.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
enum{STEPS=20000,CAP=25000,SNAPSHOTS=64};
static uint64_t state=UINT64_C(0x243f6a8885a308d3);
static uint64_t rng64(void){state^=state<<13;state^=state>>7;state^=state<<17;return state;}
typedef struct{FunctionalQueue q;int model[128];size_t n;}Snapshot;
int main(void){FQueueArena*a=fq_arena_create();assert(a);FunctionalQueue q=fq_empty();assert(fq_validate(q)&&fq_is_empty(q));int model[CAP];size_t begin=0,end=0;Snapshot snaps[SNAPSHOTS];size_t sn=0;
for(size_t step=0;step<STEPS;++step){bool enq=(begin==end)||((rng64()%100U)<58U);if(enq){int v=(int)(rng64()%100000U)-50000;FunctionalQueue next;assert(fq_enqueue(a,q,v,&next));model[end++]=v;q=next;}else{int got=0;FunctionalQueue next;assert(fq_dequeue(a,q,&got,&next));assert(got==model[begin++]);q=next;}assert(fq_validate(q));assert(fq_size(q)==end-begin);if(step%313U==0&&sn<SNAPSHOTS&&end-begin<=128U){snaps[sn].q=q;snaps[sn].n=end-begin;memcpy(snaps[sn].model,model+begin,(end-begin)*sizeof(int));++sn;}}
for(size_t s=0;s<sn;++s){FunctionalQueue cur=snaps[s].q;for(size_t i=0;i<snaps[s].n;++i){int got=0;FunctionalQueue next;assert(fq_dequeue(a,cur,&got,&next));assert(got==snaps[s].model[i]);cur=next;}assert(fq_is_empty(cur));}
FunctionalQueue v0=fq_empty(),v1,v2;assert(fq_enqueue(a,v0,10,&v1));assert(fq_enqueue(a,v1,20,&v2));int x=0;FunctionalQueue norm;assert(fq_peek(a,v1,&x,&norm)&&x==10);assert(fq_size(v1)==1&&fq_size(v2)==2);
printf("Functional queue tests passed; arena_nodes=%zu final_size=%zu snapshots=%zu\n",fq_arena_allocated_nodes(a),fq_size(q),sn);fq_arena_free(a);return 0;}
