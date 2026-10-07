# Invariants

1. Active and free slots partition the process table.
2. Every READY process appears in exactly one matching-priority queue.
3. No BLOCKED/RUNNING process appears in a ready queue.
4. At most one RUNNING process exists.
5. running_slot is NONE iff no process is RUNNING.
6. Ready queue head/tail/count agree with traversal.
7. Free stack contains only inactive slots.
8. active_count + free_top = capacity.
9. Active PID generation is nonzero.
10. Reused slot receives a new generation.
