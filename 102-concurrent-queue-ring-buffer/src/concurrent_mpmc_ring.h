#ifndef CONCURRENT_MPMC_RING_H
#define CONCURRENT_MPMC_RING_H
#include <stdbool.h>
#include <stddef.h>
typedef struct ConcurrentMpmcRing ConcurrentMpmcRing;
ConcurrentMpmcRing *cmr_create(size_t capacity_power_of_two);
void cmr_free(ConcurrentMpmcRing *ring);
bool cmr_platform_lock_free(const ConcurrentMpmcRing *ring);
size_t cmr_capacity(const ConcurrentMpmcRing *ring);
bool cmr_try_push(ConcurrentMpmcRing *ring,int value,bool *out_pushed);
bool cmr_try_pop(ConcurrentMpmcRing *ring,int *out_value,bool *out_popped);
bool cmr_size_quiescent(const ConcurrentMpmcRing *ring,size_t *out_size);
bool cmr_validate_quiescent(const ConcurrentMpmcRing *ring);
#endif
