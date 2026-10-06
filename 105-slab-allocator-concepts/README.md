# Chapter 105 — Slab Allocator Concepts

Slab allocator จัด objects เป็น **size classes** แล้วจัดหลาย slots ที่มีขนาดเท่ากันลงใน slab page เดียว. บทนี้เชื่อม fixed Object Pool ของ Chapter 103 กับ allocator ที่รองรับหลาย object sizes.

Teaching implementation ใช้ size classes:

```text
16, 32, 64, 128, 256 bytes
```

แต่ละ class มี linked list ของ slabs และแต่ละ slab มี:
- contiguous payload
- free-index stack
- in-use metadata
- requested-size metadata ต่อ slot

## Allocation

Request ถูกปัดขึ้นไป class ที่เล็กที่สุดที่ใส่ได้:

```text
1..16   -> 16
17..32  -> 32
33..64  -> 64
65..128 -> 128
129..256 -> 256
```

ถ้า class ไม่มี free slot จะสร้าง slab ใหม่.

## Internal Fragmentation

Implementation เก็บทั้ง:
- requested live bytes
- allocated class bytes
- reserved payload bytes

ดังนั้นวัดได้ว่า:

`live internal fragmentation = allocated_class_bytes - requested_live_bytes`

และแยกออกจาก reserved-but-unused capacity.

## Empty Slab Trimming

`slab_trim_empty` คืน slab ที่ live=0 กลับ system allocator. ก่อน trim allocator อาจ retain empty slabs เพื่อ reuse เร็ว.

## Tests

- **80,000 randomized allocations/frees**
- active set สูงสุด 4,096 objects
- requests 1..256 bytes
- exact class-size model
- requested/class byte accounting
- double-free rejection
- foreign-pointer rejection
- trim-empty verification
- ASan/UBSan + LeakSanitizer

## Complexity

Allocation scan ใน teaching implementation อาจเดิน slabs ใน class จนเจอ free slot. Release ต้องค้นหา slab ที่ owns pointer ดังนั้นยังไม่ใช่ production O(1) slab allocator. จุดประสงค์คือทำ concepts/invariants ให้เห็นชัดก่อน optimization.

## Build

```bash
cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
cmake --build build-asan --target ch105_slab_tests ch105_slab_benchmark
ctest --test-dir build-asan -R "^ch105_" --output-on-failure
```
