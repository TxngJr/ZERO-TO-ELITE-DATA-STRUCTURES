# Chapter 115 — Operating-System Data Structures

บทนี้สร้าง teaching model ของ **process table + scheduler run queues** สำหรับ single-CPU kernel model.

โครงสร้างหลักคือ `OsScheduler`:
- fixed-capacity process table
- generation-safe PID handles
- 4 priority levels
- FIFO ready queue ต่อ priority
- process states: READY / RUNNING / BLOCKED / UNUSED
- spawn / dispatch / yield / block / wake / terminate
- priority change
- full structural validator

## Generation-Safe PID

PID ไม่ได้เป็น slot index ตรง ๆ แต่ pack:

```text
high 32 bits = generation
low  32 bits = slot
```

ถ้า process terminate แล้ว slot ถูก reuse, generation จะเพิ่ม ทำให้ PID เก่าไม่สามารถอ้าง process ใหม่โดยบังเอิญ.

## Scheduler

Priority 0 สูงสุด. Dispatch เลือก queue แรกที่ไม่ว่างและ pop FIFO.

```text
P0: A -> B -> C
P1: D -> E
P2: ...
P3: ...
```

Model นี้ intentionally strict-priority และจึงสามารถ starve lower priorities ได้. นี่เป็นประเด็นให้เรียนรู้ ไม่ใช่ production fairness policy.

## Tests

- spawn 10,000 processes
- exact ready counts across 4 queues
- 1,000 dispatch/yield/block transitions
- wake blocked processes
- stale PID rejection after slot reuse
- priority migration
- full queue/table/free-stack validation
- ASan/UBSan

## Complexity

- spawn: O(1)
- dispatch: O(priority levels) = O(1) for 4 levels
- yield/block/wake: O(1)
- terminate READY process: O(length of its run queue) in this teaching model
- priority change for READY process: O(queue length)
- validate: O(capacity + ready entries)

This is a single-threaded scheduler model, not a concurrent kernel scheduler.
