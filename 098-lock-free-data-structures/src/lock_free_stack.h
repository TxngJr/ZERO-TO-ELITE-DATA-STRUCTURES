#ifndef LOCK_FREE_STACK_H
#define LOCK_FREE_STACK_H
#include <stdbool.h>
#include <stddef.h>
typedef struct LockFreeStack LockFreeStack;
LockFreeStack*lfs_create(size_t max_pushes);
void lfs_free(LockFreeStack*s);
bool lfs_platform_lock_free(const LockFreeStack*s);
bool lfs_push(LockFreeStack*s,int value,bool*out_pushed);
bool lfs_pop(LockFreeStack*s,int*out_value,bool*out_popped);
size_t lfs_size_approx(const LockFreeStack*s);
size_t lfs_reserved_slots(const LockFreeStack*s);
size_t lfs_push_cas_failures(const LockFreeStack*s);
size_t lfs_pop_cas_failures(const LockFreeStack*s);
bool lfs_validate_quiescent(const LockFreeStack*s);
#endif
