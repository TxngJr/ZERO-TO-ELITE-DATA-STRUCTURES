#include "functional_queue.h"
#include <stdint.h>
#include <stdlib.h>
enum{FQ_BLOCK_CAPACITY=256};
struct FQueueNode{int value;const struct FQueueNode*next;};
typedef struct FQueueBlock{struct FQueueBlock*next;size_t used;struct FQueueNode nodes[FQ_BLOCK_CAPACITY];}FQueueBlock;
struct FQueueArena{FQueueBlock*head;size_t node_count;};
typedef struct{FQueueBlock*head;size_t head_used,node_count;}ArenaMark;
static ArenaMark mark_arena(const FQueueArena*a){ArenaMark m={a->head,a->head?a->head->used:0U,a->node_count};return m;}
static void rollback_arena(FQueueArena*a,ArenaMark m){while(a->head!=m.head){FQueueBlock*d=a->head;a->head=d->next;free(d);}if(a->head)a->head->used=m.head_used;a->node_count=m.node_count;}
static FQueueNode*alloc_node(FQueueArena*a,int value,const FQueueNode*next){if(!a->head||a->head->used==FQ_BLOCK_CAPACITY){FQueueBlock*b=calloc(1,sizeof*b);if(!b)return NULL;b->next=a->head;a->head=b;}FQueueNode*n=&a->head->nodes[a->head->used++];n->value=value;n->next=next;++a->node_count;return n;}
FQueueArena*fq_arena_create(void){return calloc(1,sizeof(FQueueArena));}
void fq_arena_free(FQueueArena*a){if(!a)return;while(a->head){FQueueBlock*n=a->head->next;free(a->head);a->head=n;}free(a);}
size_t fq_arena_allocated_nodes(const FQueueArena*a){return a?a->node_count:0U;}
FunctionalQueue fq_empty(void){FunctionalQueue q={0};return q;}
size_t fq_size(FunctionalQueue q){if(q.front_len>SIZE_MAX-q.rear_len)return SIZE_MAX;return q.front_len+q.rear_len;}
bool fq_is_empty(FunctionalQueue q){return q.front_len==0U&&q.rear_len==0U;}
static size_t list_length(const FQueueNode*n){size_t len=0;while(n){if(len==SIZE_MAX)return SIZE_MAX;++len;n=n->next;}return len;}
static bool shallow_valid(FunctionalQueue q){if((q.front==NULL)!=(q.front_len==0U))return false;if((q.rear==NULL)!=(q.rear_len==0U))return false;return q.front_len<=SIZE_MAX-q.rear_len;}
bool fq_validate(FunctionalQueue q){if(!shallow_valid(q))return false;return list_length(q.front)==q.front_len&&list_length(q.rear)==q.rear_len;}
bool fq_enqueue(FQueueArena*a,FunctionalQueue q,int value,FunctionalQueue*out){if(!a||!out||!shallow_valid(q)||q.rear_len==SIZE_MAX)return false;ArenaMark m=mark_arena(a);FQueueNode*n=alloc_node(a,value,q.rear);if(!n){rollback_arena(a,m);return false;}*out=q;out->rear=n;out->rear_len=q.rear_len+1U;return true;}
static bool normalize(FQueueArena*a,FunctionalQueue q,FunctionalQueue*out){if(q.front_len!=0U||q.rear_len==0U){*out=q;return true;}ArenaMark m=mark_arena(a);const FQueueNode*src=q.rear,*front=NULL;size_t count=0;while(src){FQueueNode*n=alloc_node(a,src->value,front);if(!n){rollback_arena(a,m);return false;}front=n;++count;src=src->next;}if(count!=q.rear_len){rollback_arena(a,m);return false;}out->front=front;out->front_len=q.rear_len;out->rear=NULL;out->rear_len=0U;return true;}
bool fq_peek(FQueueArena*a,FunctionalQueue q,int*out_value,FunctionalQueue*out_normalized){if(!a||!out_value||!out_normalized||!shallow_valid(q)||fq_is_empty(q))return false;FunctionalQueue n;if(!normalize(a,q,&n)||!n.front)return false;*out_value=n.front->value;*out_normalized=n;return true;}
bool fq_dequeue(FQueueArena*a,FunctionalQueue q,int*out_value,FunctionalQueue*out){if(!a||!out_value||!out||!shallow_valid(q)||fq_is_empty(q))return false;FunctionalQueue n;if(!normalize(a,q,&n)||!n.front)return false;*out_value=n.front->value;out->front=n.front->next;out->front_len=n.front_len-1U;out->rear=n.rear;out->rear_len=n.rear_len;return true;}
