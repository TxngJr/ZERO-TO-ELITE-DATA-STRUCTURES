# Implementation Notes

`ObjectPool` ใช้:
- aligned contiguous payload storage
- `size_t free_stack[]`
- byte-per-slot `in_use[]`

Pointer validation แปลง address เป็น `uintptr_t`, ตรวจ range และตรวจ `delta % stride == 0`. วิธีนี้หลีกเลี่ยง relational pointer arithmetic ระหว่าง unrelated objects.

`aligned_alloc` ใช้ size ที่เป็น multiple ของ alignment เพราะ `stride * capacity` เป็น multiple ของ `alignof(max_align_t)`.
