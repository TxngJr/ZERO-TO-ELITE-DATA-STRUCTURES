# Exercises

## Beginner
1. Dataset row-major layout คืออะไร?
2. Batch permutation ใช้ทำอะไร?
3. Batch size 128 กับ 1,000 rows มีกี่ batches?
4. Embedding row คืออะไร?
5. Gather ต่างจาก row view อย่างไร?

## Intermediate
6. เพิ่ม drop-last batch option.
7. เพิ่ม sequential/non-shuffled plan.
8. เพิ่ม multi-label rows.
9. เพิ่ม feature masks.
10. เพิ่ม embedding padding row.

## Advanced
11. เพิ่ม stratified batch plan.
12. เพิ่ม bucketed batching by sequence length.
13. ออกแบบ sparse CSR feature dataset.
14. เพิ่ม memory-mapped dataset concept.
15. วิเคราะห์ gather locality จาก shuffled indices.

## Implementation
16. เพิ่ม batch prefetch queue.
17. เพิ่ม weighted sampling plan.
18. เพิ่ม dataset split views.

## Challenge / Research
19. เปรียบเทียบ AoS/SoA layouts สำหรับ ML features.
20. เชื่อม Chapter 118 โดยสร้าง vector-search index จาก embedding rows.
