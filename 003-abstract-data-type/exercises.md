# Exercises — Chapter 003

## Beginner
1. นิยาม ADT ด้วยภาษาของตัวเอง
2. อธิบาย Data Structure vs ADT
3. เขียน operations ของ Queue ADT
4. ระบุ observable behavior ของ Stack
5. ยกตัวอย่าง implementation สองแบบของ Stack

## Intermediate
6. เขียน invariant ของ array-backed Stack
7. อธิบายว่าทำไม capacity ไม่ควรอยู่ใน behavior contract ทั่วไป
8. ระบุ pre/postcondition ของ push
9. ระบุ pre/postcondition ของ pop
10. อธิบาย opaque type ใน C

## Advanced
11. ออกแบบ Set ADT โดยยังไม่เลือก hash/tree
12. อธิบาย representation function ของ stack implementation ในบท
13. วิเคราะห์ว่าการ expose data pointer ทำลาย abstraction อย่างไร
14. ออกแบบ error contract สำหรับ allocation failure
15. อธิบาย strong guarantee ตอน grow ด้วย temporary pointer

## Implementation
16. เพิ่ม int_stack_clear โดยรักษา allocation ไว้
17. เพิ่ม int_stack_clone พร้อมจัดการ allocation failure
18. สร้าง fixed-capacity Stack implementation ที่ใช้ test behavior ชุดใกล้เคียงเดิม

## Challenge / Research
19. เปรียบเทียบ ADT กับ interface/protocol/trait ใน C++, Java, Rust หรือ Go
20. เขียน specification เชิงสมการอย่างง่ายของ Queue: empty, enqueue, front, dequeue
