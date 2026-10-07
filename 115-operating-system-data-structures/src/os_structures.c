#include "os_structures.h"
#include <limits.h>
#include <stdlib.h>

enum { NONE = SIZE_MAX };

typedef struct {
    uint32_t generation;
    uint8_t priority;
    OsProcState state;
    size_t next_ready;
    bool active;
} Process;

typedef struct {
    size_t head;
    size_t tail;
    size_t count;
} RunQueue;

struct OsScheduler {
    size_t capacity;
    size_t active_count;
    size_t free_top;
    size_t running_slot;
    Process *procs;
    size_t *free_stack;
    RunQueue queues[OS_PRIORITY_LEVELS];
};

static OsPid make_pid(size_t slot, uint32_t generation) {
    return ((uint64_t)generation << 32U) | (uint64_t)(uint32_t)slot;
}

static bool decode(const OsScheduler *os, OsPid pid, size_t *out_slot) {
    if (!os) return false;

    size_t slot = (size_t)(uint32_t)pid;
    uint32_t generation = (uint32_t)(pid >> 32U);
    if (slot >= os->capacity || generation == 0U) return false;

    const Process *proc = &os->procs[slot];
    if (!proc->active || proc->generation != generation) return false;

    if (out_slot) *out_slot = slot;
    return true;
}

static void queue_push(OsScheduler *os, size_t slot) {
    RunQueue *queue = &os->queues[os->procs[slot].priority];
    os->procs[slot].next_ready = NONE;

    if (queue->tail == NONE) {
        queue->head = slot;
    } else {
        os->procs[queue->tail].next_ready = slot;
    }

    queue->tail = slot;
    ++queue->count;
}

static size_t queue_pop(OsScheduler *os, uint8_t priority) {
    RunQueue *queue = &os->queues[priority];
    if (queue->head == NONE) return NONE;

    size_t slot = queue->head;
    queue->head = os->procs[slot].next_ready;
    os->procs[slot].next_ready = NONE;
    --queue->count;

    if (queue->head == NONE) queue->tail = NONE;
    return slot;
}

static bool queue_remove(OsScheduler *os, size_t slot) {
    uint8_t priority = os->procs[slot].priority;
    RunQueue *queue = &os->queues[priority];
    size_t previous = NONE;
    size_t current = queue->head;

    while (current != NONE && current != slot) {
        previous = current;
        current = os->procs[current].next_ready;
    }

    if (current == NONE) return false;

    size_t next = os->procs[current].next_ready;
    if (previous == NONE) {
        queue->head = next;
    } else {
        os->procs[previous].next_ready = next;
    }

    if (queue->tail == current) queue->tail = previous;
    os->procs[current].next_ready = NONE;
    --queue->count;
    return true;
}

OsScheduler *os_create(size_t capacity) {
    if (capacity == 0U || capacity > UINT32_MAX ||
        capacity > SIZE_MAX / sizeof(Process) ||
        capacity > SIZE_MAX / sizeof(size_t)) {
        return NULL;
    }

    OsScheduler *os = calloc(1, sizeof(*os));
    if (!os) return NULL;

    os->procs = calloc(capacity, sizeof(*os->procs));
    os->free_stack = malloc(capacity * sizeof(*os->free_stack));
    if (!os->procs || !os->free_stack) {
        free(os->procs);
        free(os->free_stack);
        free(os);
        return NULL;
    }

    os->capacity = capacity;
    os->free_top = capacity;
    os->running_slot = NONE;

    for (size_t i = 0; i < capacity; ++i)
        os->free_stack[i] = capacity - 1U - i;

    for (size_t p = 0; p < OS_PRIORITY_LEVELS; ++p)
        os->queues[p] = (RunQueue){NONE, NONE, 0U};

    return os;
}

void os_free(OsScheduler *os) {
    if (!os) return;
    free(os->procs);
    free(os->free_stack);
    free(os);
}

bool os_spawn(OsScheduler *os, uint8_t priority, OsPid *out_pid) {
    if (!os || !out_pid || priority >= OS_PRIORITY_LEVELS ||
        os->free_top == 0U) {
        return false;
    }

    size_t slot = os->free_stack[--os->free_top];
    Process *proc = &os->procs[slot];

    ++proc->generation;
    if (proc->generation == 0U) ++proc->generation;

    proc->priority = priority;
    proc->state = OS_PROC_READY;
    proc->active = true;
    proc->next_ready = NONE;

    ++os->active_count;
    queue_push(os, slot);
    *out_pid = make_pid(slot, proc->generation);
    return true;
}

bool os_dispatch(OsScheduler *os, OsPid *out_pid, bool *out_dispatched) {
    if (!os || !out_pid || !out_dispatched) return false;

    if (os->running_slot != NONE) {
        *out_dispatched = false;
        return true;
    }

    for (uint8_t priority = 0; priority < OS_PRIORITY_LEVELS; ++priority) {
        size_t slot = queue_pop(os, priority);
        if (slot == NONE) continue;

        os->procs[slot].state = OS_PROC_RUNNING;
        os->running_slot = slot;
        *out_pid = make_pid(slot, os->procs[slot].generation);
        *out_dispatched = true;
        return true;
    }

    *out_dispatched = false;
    return true;
}

bool os_yield(OsScheduler *os, OsPid pid) {
    size_t slot = 0U;
    if (!decode(os, pid, &slot) || os->running_slot != slot ||
        os->procs[slot].state != OS_PROC_RUNNING) {
        return false;
    }

    os->running_slot = NONE;
    os->procs[slot].state = OS_PROC_READY;
    queue_push(os, slot);
    return true;
}

bool os_block(OsScheduler *os, OsPid pid) {
    size_t slot = 0U;
    if (!decode(os, pid, &slot) || os->running_slot != slot ||
        os->procs[slot].state != OS_PROC_RUNNING) {
        return false;
    }

    os->running_slot = NONE;
    os->procs[slot].state = OS_PROC_BLOCKED;
    return true;
}

bool os_wake(OsScheduler *os, OsPid pid) {
    size_t slot = 0U;
    if (!decode(os, pid, &slot) ||
        os->procs[slot].state != OS_PROC_BLOCKED) {
        return false;
    }

    os->procs[slot].state = OS_PROC_READY;
    queue_push(os, slot);
    return true;
}

bool os_terminate(OsScheduler *os, OsPid pid) {
    size_t slot = 0U;
    if (!decode(os, pid, &slot)) return false;

    Process *proc = &os->procs[slot];
    if (proc->state == OS_PROC_READY && !queue_remove(os, slot))
        return false;

    if (proc->state == OS_PROC_RUNNING) {
        if (os->running_slot != slot) return false;
        os->running_slot = NONE;
    }

    proc->active = false;
    proc->state = OS_PROC_UNUSED;
    proc->next_ready = NONE;
    --os->active_count;
    os->free_stack[os->free_top++] = slot;
    return true;
}

bool os_set_priority(OsScheduler *os, OsPid pid, uint8_t priority) {
    size_t slot = 0U;
    if (priority >= OS_PRIORITY_LEVELS || !decode(os, pid, &slot))
        return false;

    Process *proc = &os->procs[slot];
    if (proc->priority == priority) return true;

    if (proc->state == OS_PROC_READY) {
        if (!queue_remove(os, slot)) return false;
        proc->priority = priority;
        queue_push(os, slot);
    } else {
        proc->priority = priority;
    }
    return true;
}

bool os_get_state(const OsScheduler *os, OsPid pid,
                  OsProcState *out_state, uint8_t *out_priority) {
    size_t slot = 0U;
    if (!out_state || !decode(os, pid, &slot)) return false;
    *out_state = os->procs[slot].state;
    if (out_priority) *out_priority = os->procs[slot].priority;
    return true;
}

size_t os_process_count(const OsScheduler *os) {
    return os ? os->active_count : 0U;
}

size_t os_ready_count(const OsScheduler *os, uint8_t priority) {
    return os && priority < OS_PRIORITY_LEVELS
        ? os->queues[priority].count
        : 0U;
}

bool os_validate(const OsScheduler *os) {
    if (!os || !os->procs || !os->free_stack || os->capacity == 0U ||
        os->free_top > os->capacity || os->active_count > os->capacity) {
        return false;
    }

    unsigned char *seen = calloc(os->capacity, 1U);
    if (!seen) return false;

    size_t ready_total = 0U;
    size_t active = 0U;
    size_t running = 0U;

    for (uint8_t priority = 0; priority < OS_PRIORITY_LEVELS; ++priority) {
        const RunQueue *queue = &os->queues[priority];
        size_t count = 0U;
        size_t current = queue->head;
        size_t last = NONE;

        while (current != NONE) {
            if (current >= os->capacity || seen[current]) {
                free(seen);
                return false;
            }

            const Process *proc = &os->procs[current];
            if (!proc->active || proc->state != OS_PROC_READY ||
                proc->priority != priority) {
                free(seen);
                return false;
            }

            seen[current] = 1U;
            ++count;
            last = current;
            current = proc->next_ready;
            if (count > os->capacity) {
                free(seen);
                return false;
            }
        }

        if (count != queue->count ||
            ((count == 0U) != (queue->tail == NONE)) ||
            (count != 0U && last != queue->tail)) {
            free(seen);
            return false;
        }
        ready_total += count;
    }

    for (size_t i = 0; i < os->capacity; ++i) {
        const Process *proc = &os->procs[i];
        if (proc->active) {
            ++active;
            if (proc->state == OS_PROC_RUNNING) {
                ++running;
                if (os->running_slot != i) {
                    free(seen);
                    return false;
                }
            } else if (proc->state == OS_PROC_READY) {
                if (!seen[i]) {
                    free(seen);
                    return false;
                }
            } else if (proc->state != OS_PROC_BLOCKED) {
                free(seen);
                return false;
            }
        } else if (proc->state != OS_PROC_UNUSED) {
            free(seen);
            return false;
        }
    }

    if (active != os->active_count || running > 1U ||
        ((running == 0U) != (os->running_slot == NONE)) ||
        ready_total > active) {
        free(seen);
        return false;
    }

    for (size_t i = 0; i < os->free_top; ++i) {
        size_t slot = os->free_stack[i];
        if (slot >= os->capacity || seen[slot] || os->procs[slot].active) {
            free(seen);
            return false;
        }
        seen[slot] = 2U;
    }

    bool ok = os->free_top + os->active_count == os->capacity;
    free(seen);
    return ok;
}
