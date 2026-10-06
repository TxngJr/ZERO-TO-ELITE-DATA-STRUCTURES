# Exercises

## Beginner
1. Trace first-fit บน free blocks 64,128,32.
2. อธิบาย prefix/suffix split.
3. External fragmentation คืออะไร?
4. Coalescing ทำไมสำคัญ?
5. Largest-free-block metric บอกอะไร?

## Intermediate
6. เพิ่ม best-fit policy.
7. เพิ่ม next-fit cursor.
8. วัด free-block count หลัง workload.
9. เพิ่ม fragmentation ratio.
10. เพิ่ม allocation-size histogram.

## Advanced
11. เปลี่ยน metadata ไปอยู่ใน managed region.
12. เพิ่ม boundary tags.
13. เพิ่ม size-segregated bins.
14. วิเคราะห์ best-fit vs first-fit.
15. เพิ่ม red-black tree ของ free blocks.

## Implementation
16. เพิ่ม realloc.
17. เพิ่ม aligned realloc.
18. เพิ่ม fault-injection metadata tests.

## Challenge / Research
19. ศึกษา dlmalloc/jemalloc concepts.
20. เปรียบเทียบ Free List กับ Slab ใน mixed-size workload.
