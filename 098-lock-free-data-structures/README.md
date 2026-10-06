# Chapter 098 — Lock-Free Data Structures

บทนี้เข้าสู่ non-blocking progress จริง โดยใช้ bounded teaching implementation ของ **Treiber-style lock-free stack**.

Implementation ใช้ atomic head index + CAS retry loop. ทุก push จอง node slot ใหม่จาก fixed pool และ **ไม่ reuse/reclaim popped nodes ระหว่างอายุ stack**. การตัด node reuse ออกเป็น design choice สำคัญเพื่อไม่ให้บทนี้แอบซ่อน ABA + safe memory reclamation ก่อนถึงบทที่เกี่ยวข้อง.

## Progress Contract

Lock-free หมายถึง system โดยรวมต้องมี operation คืบหน้า แม้ thread ใด thread หนึ่งอาจ retry/starve.

CAS loop:

```text
read head
prepare next
CAS(head, old, new)
  success -> done
  failure -> another operation changed shared state; retry
```

ไม่มี mutex แต่ contention ยังทำให้ CAS failures สูงได้.

## Platform Assumption

C atomics ไม่รับประกันว่าทุก type เป็น hardware lock-free. `lfs_platform_lock_free` ตรวจ atomics ที่ implementation ใช้. Tests ของบท require ผลเป็น true บน CI platform ก่อนอ้าง progress guarantee นี้.

## ABA / Reclamation

Classic pointer Treiber stack มีปัญหา ABA และ reclamation: thread อาจถือ pointer ไป node ที่ถูก pop/free/reuse.

บทนี้หลีกเลี่ยงโดย:
- node slot ถูกใช้กับ push เดียวตลอด lifetime
- popped node ไม่ถูก reuse
- free ทั้ง pool เมื่อไม่มี worker แล้วเท่านั้น

ผลคือ memory usage ถูก bound ด้วย **max lifetime pushes**, ไม่ใช่ current stack size.

## Approximate Size

`size` เป็น diagnostic metadata. Push increment size ก่อน publish head เพื่อไม่ให้ pop ที่เห็น new head decrement จาก zero. ระหว่าง in-flight push ค่าอาจรวม node ที่ยังไม่ publish; หลัง quiescence จึง exact.

## Tests

- 8 threads push 40,000 unique values
- 8 threads pop และ verify exactly-once
- mixed 4 producers + 4 consumers
- CAS failure counters
- ASan/UBSan + TSan

## Complexity

Ignoring contention, push/pop expected constant local work per CAS attempt. Number of retries ไม่ bounded ต่อ thread จึงไม่ใช่ wait-free.

## Build

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch098_lock_free_tests
ctest --test-dir build-tsan -R "^ch098_" --output-on-failure
```
