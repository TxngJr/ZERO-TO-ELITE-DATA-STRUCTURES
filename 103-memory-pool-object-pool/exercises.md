# Exercises

## Beginner
1. คำนวณ stride ของ object 24 bytes.
2. Trace free stack 4 slots.
3. Allocate จน full แล้วเกิดอะไร?
4. Release แล้ว pointer เดิม reuse ได้ไหม?
5. ownership check ต้องตรวจอะไรบ้าง?

## Intermediate
6. เพิ่ม high-water mark.
7. เพิ่ม poison-on-free debug mode.
8. เพิ่ม per-slot generation handle.
9. วัด padding waste หลาย object sizes.
10. เปรียบเทียบ malloc/free throughput.

## Advanced
11. ออกแบบ growable pool.
12. วิเคราะห์ cache locality.
13. เพิ่ม bitmap แทน byte in_use.
14. ออกแบบ thread-safe pool.
15. อธิบาย ABA ถ้า free slots reuse ใน lock-free pool.

## Implementation
16. เพิ่ม calloc-style constructor hook.
17. เพิ่ม iterator over live slots.
18. เพิ่ม fault-injection construction test.

## Challenge / Research
19. เปรียบเทียบ pool กับ slab allocator.
20. ออกแบบ handle = index+generation เพื่อตรวจ stale reference.
