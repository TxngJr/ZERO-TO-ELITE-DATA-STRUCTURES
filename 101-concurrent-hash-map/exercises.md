# Exercises

## Beginner
1. อธิบาย bucket lock.
2. อธิบาย table RW-lock.
3. ทำไม bucket_count เป็น power of two?
4. Load factor คืออะไร?
5. Resize ย้าย key อย่างไร?

## Intermediate
6. เพิ่ม contains.
7. เพิ่ม insert-if-absent.
8. เพิ่ม atomic update-if-present.
9. เปลี่ยน resize threshold 2/4/8 แล้ว benchmark.
10. วัด bucket chain distribution.

## Advanced
11. วิเคราะห์ writer starvation ของ RW-lock.
12. ออกแบบ shrink protocol.
13. เพิ่ม snapshot ที่ consistent.
14. ออกแบบ incremental resize.
15. วิเคราะห์ lock ordering ของ two-key operation.

## Implementation
16. เพิ่ม collision statistics.
17. เพิ่ม allocation-failure injection ตอน resize.
18. เพิ่ม concurrent readers ระหว่าง resizing stress.

## Challenge / Research
19. เปรียบเทียบ Java-style forwarding-node resize กับ global resize barrier.
20. ออกแบบ split-ordered-list approach ระดับ high level.
