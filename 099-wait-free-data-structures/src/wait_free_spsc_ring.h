#ifndef WAIT_FREE_SPSC_RING_H
#define WAIT_FREE_SPSC_RING_H
#include <stdbool.h>
#include <stddef.h>
typedef struct WaitFreeSpscRing WaitFreeSpscRing;
WaitFreeSpscRing*wfr_create(size_t physical_capacity);
void wfr_free(WaitFreeSpscRing*r);
bool wfr_platform_lock_free(const WaitFreeSpscRing*r);
size_t wfr_usable_capacity(const WaitFreeSpscRing*r);
bool wfr_try_push(WaitFreeSpscRing*r,int value,bool*out_pushed);
bool wfr_try_pop(WaitFreeSpscRing*r,int*out_value,bool*out_popped);
bool wfr_validate_quiescent(const WaitFreeSpscRing*r);
#endif
