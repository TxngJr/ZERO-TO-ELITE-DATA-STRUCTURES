# Exercises — Chapter 006

## Beginner
1. valid indices ของ array length 10 คืออะไร
2. วาด memory ของ int[5]
3. อธิบาย random access
4. แยก size กับ capacity
5. อธิบาย row-major

## Intermediate
6. อธิบาย array-to-pointer decay
7. ทำไม sizeof parameter array ใน C ไม่ให้ caller length
8. วิเคราะห์ insert index i
9. วิเคราะห์ erase index i
10. อธิบาย reserve

## Advanced
11. derive amortized append แบบ doubling
12. วิเคราะห์ growth policy +1
13. อธิบาย pointer invalidation หลัง realloc
14. อธิบาย one-past pointer และข้อห้าม dereference
15. เปรียบเทียบ static array กับ dynamic array ใน embedded-like workload

## Implementation
16. เพิ่ม contains ให้ IntVector
17. เพิ่ม remove-first
18. เพิ่ม reverse in-place โดย auxiliary Θ(1)

## Challenge / Research
19. เปรียบเทียบ growth factor 1.5 กับ 2.0 ด้าน realloc frequency และ unused capacity เชิงแนวคิด
20. ศึกษา std::vector, Rust Vec, Java ArrayList หรือ Go slice อย่างน้อยสองตัว แล้วสรุป API/invalidation/growth semantics โดยแยก documented guarantees ออกจาก implementation detail
