# Chapter 099 — Wait-Free Data Structures

บทนี้ยกระดับจาก lock-free ไปเป็น **wait-free per operation** ด้วย SPSC (Single Producer / Single Consumer) bounded ring.

`WaitFreeSpscRing` มี exactly one producer เรียก `try_push` และ exactly one consumer เรียก `try_pop`. แต่ละ call ทำจำนวนขั้นตอนคงที่ ไม่มี CAS retry loop และคืนทันทีว่า success/full/empty.

## Important Scope

Guarantee นี้ขึ้นกับ:
- caller รักษา single-producer/single-consumer contract
- atomic size_t ที่ใช้รายงานว่า lock-free บน platform
- operation ที่พูดถึงคือ `try_push/try_pop` เอง

Caller อาจเขียน loop retry เมื่อ full/empty; loop นั้นไม่ใช่ส่วนหนึ่งของ wait-free bound ของ individual try operation.

## Representation

Ring ใช้ head/tail atomics:

```text
consumer owns head writes
producer owns tail writes
one slot reserved

empty: head == tail
full : next(tail) == head
```

Producer:
1. read own tail relaxed
2. read head acquire
3. if full return false
4. write data[tail]
5. publish new tail release

Consumer ทำสมมาตรกับ head.

## Why One Slot Is Reserved

ถ้า head==tail ใช้แทน empty เราต้องมีวิธีแยก full. Design นี้ยอมเสียหนึ่ง physical slot เพื่อให้ state simple:
`usable_capacity = physical_capacity - 1`.

## Wait-Free vs Lock-Free

- Chapter 098 CAS loop อาจ retry unbounded สำหรับ thread หนึ่ง
- Chapter 099 try operation ไม่มี retry loop จึงมี bounded local steps ภายใต้ SPSC assumptions

นี่เป็น guarantee ที่แลกกับ generality: ไม่รองรับ MPMC.

## Tests

- producer/consumer transfer 500,000 ordered integers
- exact FIFO verification
- counters แยก caller-level full/empty retries
- TSan
- runtime atomic lock-free check

## Build

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch099_wait_free_tests
ctest --test-dir build-tsan -R "^ch099_" --output-on-failure
```
