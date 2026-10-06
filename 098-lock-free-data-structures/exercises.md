# Exercises

## Beginner
1. Trace push A,B,C.
2. Trace two concurrent pushes.
3. CAS failure บอกอะไร?
4. Lock-free vs blocking.
5. Lock-free vs wait-free.

## Intermediate
6. หา linearization point ของ push.
7. หา linearization point ของ pop.
8. อธิบาย release/acquire publication.
9. เพิ่ม empty query.
10. Plot CAS failures vs threads.

## Advanced
11. วาด ABA A→B→A.
12. เพิ่ม tagged head design note.
13. ศึกษา hazard pointers.
14. ศึกษา epoch reclamation.
15. วิเคราะห์ stalled thread หลัง size increment ก่อน publish.

## Implementation
16. เพิ่ม max CAS retries metric โดยไม่เปลี่ยน progress.
17. เพิ่ม mixed producer/consumer benchmark.
18. สร้าง intentionally unsafe reuse version ใน isolated lab แล้วใช้ sanitizer.

## Challenge / Research
19. เปรียบเทียบ Treiber stack กับ elimination backoff stack.
20. ออกแบบ reclamation contract ที่จะรองรับ unbounded lifetime pushes.
