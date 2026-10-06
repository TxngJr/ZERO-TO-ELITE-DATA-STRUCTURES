# Chapter 102 — Concurrent Queue / Ring Buffer

บทนี้ปิดหมวด Concurrent Structures ด้วย bounded **MPMC ring buffer** ที่ใช้ per-slot sequence numbers และ atomic enqueue/dequeue positions.

เป้าหมายคือรวมสิ่งที่เรียนมา:
- atomic publication
- CAS contention
- ring-buffer indexing
- multi-producer / multi-consumer coordination
- full/empty detection
- sequence-number ownership
- progress-claim discipline

> สำคัญ: implementation นี้ใช้ atomics และ non-blocking try API แต่เอกสาร **ไม่อ้าง formal lock-free/wait-free guarantee**. Producer ที่ reserve position แล้ว stall ก่อน publish slot สามารถทำให้ consumer หน้า queue หยุดรอ logical front ได้. “ใช้ CAS” ไม่เท่ากับ “lock-free” โดยอัตโนมัติ.

## Representation

Capacity ต้องเป็น power of two:

```text
slot = position & (capacity - 1)
```

แต่ละ cell เก็บ:
- atomic `sequence`
- payload `data`

Global:
- atomic `enqueue_pos`
- atomic `dequeue_pos`

## Producer Protocol

1. อ่าน enqueue position
2. ดู cell sequence
3. ถ้า sequence == position → slot พร้อมให้ producer claim
4. CAS enqueue position
5. เขียน payload
6. release-store sequence = position + 1 เพื่อ publish

ถ้า sequence < position ตาม no-wrap teaching contract → queue full.

## Consumer Protocol

Consumer คาด sequence = dequeue position + 1.
เมื่อ claim สำเร็จ:
1. อ่าน payload
2. release-store sequence = position + capacity
3. slot พร้อม reuse ในรอบถัดไป

## Sequence Numbers

Sequence ช่วยแยกว่า physical slot เดิมกำลังอยู่ใน generation ไหน. นี่แก้ ambiguity ที่ head/tail modulo indices อย่างเดียวไม่พอสำหรับ MPMC.

Teaching implementation กำหนด lifetime counters ไม่ wrap; เมื่อ position เข้าใกล้ `SIZE_MAX` API ปฏิเสธ operation แทนการพึ่ง signed-wrap trick.

## Tests

- 4 producers + 4 consumers
- 100,000 unique values
- exactly-once verification
- final quiescent size = 0
- runtime atomic lock-free check
- ASan/UBSan + TSan

## Complexity

แต่ละ successful/failed try มี constant local workต่อ attempt แต่ CAS contention อาจเกิด retry loop. ไม่มี blocking mutex/condition variable.

## Compare Previous Chapters

- 097: blocking MPMC queue ด้วย mutex/condition variables
- 099: wait-free SPSC ring ภายใต้ single-producer/single-consumer
- 102: general MPMC bounded ring ด้วย per-slot generations และ CAS

## Build

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch102_concurrent_ring_tests
ctest --test-dir build-tsan -R "^ch102_" --output-on-failure
```
