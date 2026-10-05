# Exercises — Chapter 010

## Beginner
1. นิยาม FIFO
2. วาด enqueue/dequeue 3 values
3. front ต่างจาก dequeue อย่างไร
4. ทำไม linked queue ต้องมี tail
5. ทำไม naive shifting queue ช้า

## Intermediate
6. คำนวณ wrapped physical index
7. อธิบาย head==tail ambiguity
8. เขียน bounded queue policy 3 แบบ
9. trace circular queue หลัง wrap
10. อธิบาย grow ของ wrapped queue

## Advanced
11. พิสูจน์ enqueue amortized Θ(1)
12. อธิบาย Queue ใน BFS
13. เปรียบเทียบ linked vs circular memory behavior
14. ออกแบบ queue API ที่คืน error code
15. อธิบาย why queue is not thread-safe by default

## Implementation
16. เพิ่ม clear
17. เพิ่ม bounded fixed queue
18. เพิ่ม bulk enqueue

## Challenge / Research
19. ศึกษา producer-consumer queue concept และแยก blocking queue จาก non-blocking queue
20. เปรียบเทียบ queue implementation ใน C++ deque, Java ArrayDeque และ Go channel ในระดับ semantics โดยไม่สรุปว่าเป็น implementation เดียวกัน
