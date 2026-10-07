#ifndef OS_STRUCTURES_H
#define OS_STRUCTURES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
enum { OS_PRIORITY_LEVELS = 4 };
typedef uint64_t OsPid;
typedef enum { OS_PROC_UNUSED=0, OS_PROC_READY, OS_PROC_RUNNING, OS_PROC_BLOCKED } OsProcState;
typedef struct OsScheduler OsScheduler;
OsScheduler *os_create(size_t max_processes);
void os_free(OsScheduler *os);
bool os_spawn(OsScheduler *os, uint8_t priority, OsPid *out_pid);
bool os_dispatch(OsScheduler *os, OsPid *out_pid, bool *out_dispatched);
bool os_yield(OsScheduler *os, OsPid pid);
bool os_block(OsScheduler *os, OsPid pid);
bool os_wake(OsScheduler *os, OsPid pid);
bool os_terminate(OsScheduler *os, OsPid pid);
bool os_set_priority(OsScheduler *os, OsPid pid, uint8_t priority);
bool os_get_state(const OsScheduler *os, OsPid pid, OsProcState *out_state, uint8_t *out_priority);
size_t os_process_count(const OsScheduler *os);
size_t os_ready_count(const OsScheduler *os, uint8_t priority);
bool os_validate(const OsScheduler *os);
#endif
