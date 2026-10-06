# Exercises

## Beginner
1. นิยาม thread.
2. นิยาม critical section.
3. Race condition vs data race.
4. อธิบาย mutex.
5. Atomicity/visibility/ordering ต่างกันอย่างไร.

## Intermediate
6. หา linearization point ของ insert.
7. ทำไม contains ต้อง lock.
8. Trace concurrent duplicate inserts.
9. เพิ่ม clear.
10. วัด 1/2/4/8 threads.

## Advanced
11. เปลี่ยนเป็น rwlock.
12. ออกแบบ 16-lock sharding.
13. สร้าง check-then-act race.
14. อธิบาย CAS retry loop.
15. วาด ABA timeline.

## Implementation
16. เพิ่ม insert_if_absent.
17. Benchmark snapshot ระหว่าง writers.
18. สร้าง intentionally-racy lab copy แล้วใช้ TSan.

## Challenge / Research
19. เปรียบเทียบ mutex/rwlock/striped/RCU-style designs.
20. เขียน lock-free set design note ที่ระบุ ABA + reclamation ก่อน implement.
