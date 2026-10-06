# Chapter 095 — Copy-on-Write Structures

Copy-on-Write (COW) ให้หลาย logical objects share backing storage ขณะที่ยังไม่มี write. Clone จึงถูกมาก; เมื่อ object หนึ่งต้อง mutate และ storage ยัง shared จึง detach/copy ก่อนเขียน.

Implementation คือ `CowIntVector`:
- clone handle Θ(1), เพิ่ม refcount
- read share storage ได้
- set/pop ถ้า unique เขียน in place
- mutation เมื่อ shared = copy payload ก่อน
- push อาจ detach เพราะ shared หรือ capacity growth

## Mental Model

```text
x ----+
      +--> storage A refs=2 [10 20 30]
y ----+

set(y,1,99)

x ------> A refs=1 [10 20 30]
y ------> B refs=1 [10 99 30]
```

## COW vs Persistence / Immutability

COW เป็น storage optimization. Logical vector ยังมี mutable API. ถ้า backing unique มัน mutate storage เดิมได้ จึงไม่เท่ากับ immutable หรือ persistent structure.

## Detach Contract

1. คำนวณ capacity แบบ overflow-safe.
2. ถ้า refs=1 และ capacity พอ → ใช้ backing เดิม.
3. Allocate backing ใหม่.
4. Copy elements.
5. ลด refs ของ backing เก่า.
6. ถ้า refs เก่าเป็น 0 ต้อง free.
7. Publish backing ใหม่ แล้วจึง mutate.

Allocation failure ต้องไม่เปลี่ยน logical object เดิม.

## Invariants

- every live handle points to one live storage
- refs = number of live handles sharing storage
- refs > 0
- size <= capacity
- mutation on shared storage detaches first
- storage freed exactly when refs becomes zero
- refcount implementation นี้เป็น non-atomic และ **ไม่ thread-safe**

## Complexity

| Operation | Unique | Shared |
|---|---:|---:|
| clone | Θ(1) | Θ(1) |
| get | Θ(1) | Θ(1) |
| set | Θ(1) | Θ(n) detach |
| push | amortized Θ(1) | Θ(n) detach/grow |
| pop | Θ(1) | Θ(n) detach |

COW เลื่อน cost จาก clone-time ไป first-write-time จึงต้อง benchmark ทั้งสองช่วง.

## Real-World Connections

COW concepts พบใน virtual-memory snapshots, buffers, strings/arrays และ snapshot systems แต่ implementation จริงแตกต่างตามระบบ/version.

## When NOT to Use

ไม่เหมาะเมื่อ clone ทุกตัวเขียนทันที, payload ใหญ่และ spike latency รับไม่ได้, sharing ต่ำ, หรือใช้ข้าม threads โดยไม่มี synchronization.

## Build

```bash
cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
cmake --build build-asan --target ch095_cow_tests ch095_cow_benchmark
ctest --test-dir build-asan -R ch095 --output-on-failure
```
