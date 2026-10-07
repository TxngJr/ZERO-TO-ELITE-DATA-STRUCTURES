# Theory — OS Data Structures

Operating systems rely on compact metadata tables and queues:

- process/thread control blocks
- run queues
- wait queues
- PID/handle tables
- free-slot structures
- page/frame tables
- file descriptor tables

This chapter focuses on process identity + scheduling state.

A slot index alone is unsafe as a long-lived handle because a terminated slot can be reused. Generation counters distinguish different lifetimes occupying the same physical slot.

Strict-priority scheduling gives simple lookup cost but may starve lower priorities. Production schedulers add fairness, vruntime, aging, deadlines or per-CPU queues.
