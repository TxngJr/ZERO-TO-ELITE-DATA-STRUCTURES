#ifndef CONCURRENT_INT_SET_H
#define CONCURRENT_INT_SET_H
#include <stdbool.h>
#include <stddef.h>
typedef struct ConcurrentIntSet ConcurrentIntSet;
ConcurrentIntSet*cis_create(void);
void cis_free(ConcurrentIntSet*set);
bool cis_insert(ConcurrentIntSet*set,int value,bool*out_inserted);
bool cis_remove(ConcurrentIntSet*set,int value,bool*out_removed);
bool cis_contains(ConcurrentIntSet*set,int value,bool*out_contains);
bool cis_size(ConcurrentIntSet*set,size_t*out_size);
bool cis_snapshot(ConcurrentIntSet*set,int**out_values,size_t*out_count);
bool cis_validate(ConcurrentIntSet*set);
#endif
