# Exercises — Chapter 008

## Beginner
1. วาด singly list 3 nodes
2. เขียน invariant ของ empty list
3. push_front เปลี่ยน pointers อะไร
4. push_back ทำไม Θ(1) เมื่อมี tail
5. get(i) ทำไมไม่ Θ(1)

## Intermediate
6. implement pop_front
7. อธิบาย pop_back singly Θ(n)
8. วาด doubly erase middle node
9. อธิบาย circular stopping condition
10. เปรียบเทียบ list กับ array locality

## Advanced
11. อธิบาย known-node vs index-based insertion complexity
12. เขียน Floyd cycle detection
13. อธิบาย sentinel trade-offs
14. อธิบาย intrusive list concept
15. วิเคราะห์ memory overhead สำหรับ payload int บน 64-bit conceptually

## Implementation
16. เพิ่ม reverse ให้ singly list
17. เพิ่ม insert_after_value
18. เพิ่ม doubly splice concept ระหว่างสอง lists

## Challenge / Research
19. ศึกษา Linux-kernel-style intrusive list concept และสรุปเหตุผลเชิง systems โดยไม่ copy source
20. เปรียบเทียบ allocator-per-node กับ arena allocation สำหรับ list workload
