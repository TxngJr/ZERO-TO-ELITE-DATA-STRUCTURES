# Exercises

## Beginner
1. Request 13 ไป class ไหน?
2. Request 33 ไป class ไหน?
3. Fragmentation ของ request 70 ใน class 128?
4. Empty slab คืออะไร?
5. Partial slab คืออะไร?

## Intermediate
6. เพิ่ม class 512.
7. เปลี่ยน slots/slab แล้ววัด reserved bytes.
8. เพิ่ม utilization per class.
9. เพิ่ม count full/partial/empty slabs.
10. วัด internal fragmentation บน random workload.

## Advanced
11. เพิ่ม partial-slab fast list.
12. เพิ่ม direct page-to-slab metadata concept.
13. ออกแบบ per-thread cache.
14. วิเคราะห์ cache coloring.
15. วิเคราะห์ NUMA-aware slab placement.

## Implementation
16. เพิ่ม trim เฉพาะ class.
17. เพิ่ม poison-on-free.
18. เพิ่ม allocation-failure injection.

## Challenge / Research
19. ศึกษา Linux SLAB/SLUB high-level design.
20. เปรียบเทียบ pool/arena/slab บน workload เดียวกัน.
