#ifndef THREAD_SAFE_STRUCTURES_H
#define THREAD_SAFE_STRUCTURES_H
#include <stdbool.h>
#include <stddef.h>
typedef struct ThreadSafeQueue ThreadSafeQueue;
typedef struct ThreadSafeIntMap ThreadSafeIntMap;
ThreadSafeQueue *tsq_create(size_t capacity);
void tsq_free(ThreadSafeQueue *q);
bool tsq_try_push(ThreadSafeQueue *q,int value,bool*out_pushed);
bool tsq_try_pop(ThreadSafeQueue *q,int*out_value,bool*out_popped);
bool tsq_push_wait(ThreadSafeQueue *q,int value,bool*out_pushed);
bool tsq_pop_wait(ThreadSafeQueue *q,int*out_value,bool*out_popped);
bool tsq_close(ThreadSafeQueue *q);
bool tsq_size(ThreadSafeQueue *q,size_t*out_size);
bool tsq_validate_quiescent(ThreadSafeQueue *q);
ThreadSafeIntMap *tsm_create(size_t bucket_count);
void tsm_free(ThreadSafeIntMap *map);
bool tsm_put(ThreadSafeIntMap *map,int key,int value,bool*out_inserted);
bool tsm_get(ThreadSafeIntMap *map,int key,int*out_value,bool*out_found);
bool tsm_remove(ThreadSafeIntMap *map,int key,bool*out_removed);
bool tsm_size(ThreadSafeIntMap *map,size_t*out_size);
bool tsm_validate_quiescent(ThreadSafeIntMap *map);
#endif
