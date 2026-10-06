# Exercises

## Beginner
1. หา slot ของ positions 0..9 เมื่อ capacity=8.
2. ทำไมต้อง power of two?
3. Sequence number ใช้ทำอะไร?
4. Producer publish เมื่อใด?
5. Consumer recycle เมื่อใด?

## Intermediate
6. Trace queue capacity=4 ด้วย 6 enqueue/dequeue.
7. อธิบาย release/acquire payload ordering.
8. เพิ่ม full/empty retry counters.
9. เปรียบเทียบ 097 blocking queue.
10. เปรียบเทียบ 099 SPSC ring.

## Advanced
11. วิเคราะห์ producer stall หลัง reserve.
12. อธิบายว่าทำไมไม่อ้าง lock-free.
13. ศึกษา counter wrap strategy.
14. วิเคราะห์ false sharing cell sequence.
15. เพิ่ม cache-line padding experiment.

## Implementation
16. เพิ่ม batch push API พร้อม semantics ชัดเจน.
17. เพิ่ม quiescent snapshot.
18. เพิ่ม contention histogram.

## Challenge / Research
19. เปรียบเทียบ sequence-number bounded MPMC algorithms หลายแบบ.
20. เขียน proof sketch ของ cell generation invariant.
