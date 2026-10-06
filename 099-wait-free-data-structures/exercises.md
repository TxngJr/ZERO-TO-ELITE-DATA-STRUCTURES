# Exercises

## Beginner
1. วาด empty ring.
2. วาด full ring เมื่อ capacity=5.
3. usable capacity เท่าไร?
4. ทำไม reserve one slot?
5. SPSC ย่อมาจากอะไร?

## Intermediate
6. Trace wrap-around.
7. หา linearization point ของ push.
8. หา linearization point ของ pop.
9. อธิบาย release tail.
10. อธิบาย acquire head.

## Advanced
11. วิเคราะห์ false sharing ของ head/tail.
12. เพิ่ม padding แยก cache line แล้ว benchmark.
13. เปรียบเทียบ SPSC กับ mutex queue.
14. อธิบาย why MPSC ต้อง protocol เพิ่ม.
15. แยก operation wait-freedom จาก caller retry loop.

## Implementation
16. เพิ่ม peek สำหรับ consumer.
17. เพิ่ม approximate occupancy สำหรับ quiescent diagnostics.
18. เพิ่ม sequence-number stress test.

## Challenge / Research
19. ศึกษา bounded MPMC ring algorithms และ progress guarantees.
20. ออกแบบ API ที่เปิด full/empty metrics โดยไม่ทำลาย fast path.
