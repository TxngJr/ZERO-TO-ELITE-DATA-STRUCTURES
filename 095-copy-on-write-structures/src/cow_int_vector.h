#ifndef COW_INT_VECTOR_H
#define COW_INT_VECTOR_H
#include <stdbool.h>
#include <stddef.h>
typedef struct CowIntVector CowIntVector;
CowIntVector*cow_vector_create(void);
CowIntVector*cow_vector_from_array(const int*values,size_t count);
CowIntVector*cow_vector_clone(const CowIntVector*src);
void cow_vector_free(CowIntVector*v);
size_t cow_vector_size(const CowIntVector*v);
size_t cow_vector_capacity(const CowIntVector*v);
size_t cow_vector_share_count(const CowIntVector*v);
bool cow_vector_get(const CowIntVector*v,size_t index,int*out);
bool cow_vector_set(CowIntVector*v,size_t index,int value);
bool cow_vector_push(CowIntVector*v,int value);
bool cow_vector_pop(CowIntVector*v,int*out);
bool cow_vector_validate(const CowIntVector*v);
#endif
