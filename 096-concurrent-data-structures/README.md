# Chapter 096 — Concurrent Data Structures

Chapter 096 เปิดหมวด Concurrent Structures โดยเริ่มจาก mental model ก่อน: thread, shared state, race condition, **C data race**, critical section, mutex, atomicity, visibility, ordering, linearizability, CAS, ABA, lock-free และ wait-free.

Implementation หลักคือ `ConcurrentIntSet`: sorted dynamic array ที่ serialize operations ด้วย `pthread_mutex_t`. Representation ถูกทำให้ง่ายโดยตั้งใจเพื่อแยก sequential correctness ออกจาก synchronization correctness. Lock-free algorithms จะเริ่มลึกใน Chapters 098–100 ไม่ใช่บทนี้.

## Thread / Race Mental Model

หลาย threads share address space. Conflicting accesses ที่อย่างน้อยหนึ่งเป็น write และไม่มี synchronization ที่สร้าง happens-before อาจเป็น data race ซึ่งใน C เป็น undefined behavior.

- **Race condition** = logic/result depends on timing/interleaving
- **Data race** = term ตาม language memory model สำหรับ unsynchronized conflicting memory accesses

ทั้งสองไม่ใช่คำเดียวกัน.

## Critical Section + Mutex

Set invariant:

```text
data[0] < data[1] < ... < data[size-1]
size <= capacity
```

Insert มีหลาย steps: binary search → possible realloc → memmove → write → size++. ทั้งชุดต้องถูกป้องกันเป็น critical section เดียว.

## Linearizability

Concurrent object แบบ linearizable ต้องสามารถ map history ไป sequential history ที่เคารพ object specification และ real-time precedence. ใน mutex implementation นี้ operation ทั้งก้อนอยู่ใน exclusive critical section จึงมี linearization point ภายในช่วง lock ที่ state ถูกอ่าน/เปลี่ยน.

## Atomicity / Visibility / Ordering

- **Atomicity**: operation/value transition ไม่ถูกเห็นแบบครึ่งกลางตาม guarantee
- **Visibility**: write ของ thread หนึ่งถูกอีก thread เห็นเมื่อใด
- **Ordering**: constraints ว่า accesses ใดต้องสังเกตก่อน/หลังกัน

Mutex ให้ synchronization boundary เมื่อใช้ถูกต้อง. C atomics ให้ control ที่ละเอียดกว่า.

## Compiler vs CPU Reordering

Compiler สามารถ transform/reorder ภายใต้ language rules; CPU มี execution/memory ordering effects ของตัวเอง. Program ต้องอาศัย language memory model + synchronization primitives ไม่ใช่ assumption จาก source order หรือ assembly run เดียว.

## Release / Acquire Publication

`atomic_publication_demo.c` ใช้:

```c
payload = 42;
atomic_store_explicit(&ready, true, memory_order_release);
```

consumer รอด้วย acquire load แล้วจึงอ่าน payload. เมื่อ acquire observes the releasing publication ตาม protocol นี้ prior producer writes ถูก ordered/visible ตาม happens-before relationship ที่เกี่ยวข้อง. Pattern นี้เป็น demo เฉพาะ ไม่ใช่สูตรสำเร็จสำหรับ arbitrary mutable structures.

## CAS

Compare-and-swap conceptually เปลี่ยนค่าก็ต่อเมื่อค่าปัจจุบันตรง expected. CAS เป็น primitive สำหรับ lock-free algorithms แต่ algorithm ยังต้องแก้ retries, memory ordering, reclamation และ ABA.

## ABA Problem

Thread อ่าน A, ถูก pause, state เปลี่ยน A→B→A, แล้ว CAS อาจเห็น “A เหมือนเดิม” ทั้งที่ structure ผ่านการเปลี่ยนแปลงมาแล้ว. Chapters lock-free ต่อไปจะลงลึก tagged state และ memory reclamation.

## Progress Guarantees

- blocking/lock-based: thread อาจรอ lock holder
- lock-free: system โดยรวมต้องมี operation progress
- wait-free: ทุก thread operation จบภายใน finite bound ตาม model

`ConcurrentIntSet` บทนี้เป็น **blocking**, ไม่ใช่ lock-free/wait-free.

## Operations

- contains: binary search under mutex
- insert/remove: search + possible shift/grow under mutex
- size: synchronized read
- snapshot: consistent copy under mutex
- validate: sorted/unique invariant under mutex

## Complexity + Contention

| Operation | Sequential work |
|---|---:|
| contains | O(log n) |
| insert/remove | O(n) worst from shifts |
| snapshot/validate | Θ(n) |

Real wall-clock latency เพิ่ม lock wait, scheduling, cache coherence และ critical-section duration; Big-O อย่างเดียวไม่อธิบาย concurrency performance.

## ThreadSanitizer

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch096_concurrent_tests ch096_atomic_publication
ctest --test-dir build-tsan -R ch096 --output-on-failure
```

TSan ช่วยหา data races แบบ dynamic แต่ **ไม่ได้พิสูจน์** linearizability, deadlock freedom หรือ correctness ของทุก schedule.

## When NOT to Use One Global Mutex

เมื่อ contention สูง, tail latency สำคัญ, blocking ยอมรับไม่ได้, หรือ workload เหมาะกับ sharding/rwlock/snapshot designs. แต่ควร optimize หลัง correctness และ measurement ไม่ใช่กระโดด lock-free ก่อนมี mental model.
