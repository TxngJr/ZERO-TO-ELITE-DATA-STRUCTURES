# Chapter 106 — Free List

บทนี้แยก **Free List** ออกจาก pool/slab เพื่อศึกษาการจัดการพื้นที่ว่างแบบ variable-size โดยตรง.

Implementation คือ `FreeListAllocator` บน contiguous managed region:
- variable-size allocations
- first-fit policy
- power-of-two alignment 1..4096
- sorted free extents
- prefix/suffix splitting
- coalescing เมื่อ release
- exact allocation tracking เพื่อ reject invalid/double free
- fragmentation metrics

## Representation

```text
managed storage
0 ------------------------------------------------ capacity

free list:
[offset,size] -> [offset,size] -> ...

allocations:
[offset,size] -> ...
```

Free list ถูก sort ตาม offset. Adjacent free extents ต้องถูก coalesce ทันที.

## Allocation

First-fit เดิน free list จนเจอ block ที่รองรับ:
1. aligned start
2. requested payload
3. optional prefix free extent
4. optional suffix free extent

กรณีมีทั้ง prefix และ suffix จะต้องสร้าง metadata เพิ่มหนึ่ง extent ก่อน mutate representation เพื่อรักษา strong failure behavior.

## Release

Release ย้าย allocation extent กลับเข้าตำแหน่ง sorted ใน free list จากนั้น:
- merge กับ successor ถ้าติดกัน
- merge กับ predecessor ถ้าติดกัน

ดังนั้นหลัง free ทุก allocation allocator ต้องกลับมาเป็น free block เดียวขนาดเท่า capacity.

## Fragmentation

Metrics:
- total free bytes
- largest free block
- free-block count

External fragmentation สามารถเกิดได้เมื่อ free bytes รวมมากพอแต่ไม่มี block เดียวใหญ่พอ.

## Tests

- 1 MiB managed region
- **50,000 randomized allocate/free operations**
- up to 2,048 live allocations
- request 1..1024 bytes
- alignment 1..64
- exact allocated-byte/live-count model
- periodic full invariant validation
- double-free + foreign pointer rejection
- final full coalescing to one extent
- ASan/UBSan + LeakSanitizer
