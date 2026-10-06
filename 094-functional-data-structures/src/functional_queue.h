#ifndef FUNCTIONAL_QUEUE_H
#define FUNCTIONAL_QUEUE_H
#include <stdbool.h>
#include <stddef.h>
typedef struct FQueueArena FQueueArena;
typedef struct FQueueNode FQueueNode;
typedef struct {const FQueueNode *front,*rear;size_t front_len,rear_len;} FunctionalQueue;
FQueueArena *fq_arena_create(void);
void fq_arena_free(FQueueArena *arena);
size_t fq_arena_allocated_nodes(const FQueueArena *arena);
FunctionalQueue fq_empty(void);
size_t fq_size(FunctionalQueue q);
bool fq_is_empty(FunctionalQueue q);
bool fq_validate(FunctionalQueue q);
bool fq_enqueue(FQueueArena *arena,FunctionalQueue q,int value,FunctionalQueue *out);
bool fq_peek(FQueueArena *arena,FunctionalQueue q,int *out_value,FunctionalQueue *out_normalized);
bool fq_dequeue(FQueueArena *arena,FunctionalQueue q,int *out_value,FunctionalQueue *out);
#endif
