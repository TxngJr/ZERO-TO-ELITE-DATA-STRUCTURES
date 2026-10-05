# Exercises — Chapter 001

## Beginner
1. อธิบายความต่างระหว่าง value, variable และ type ด้วยตัวอย่าง int
2. เขียน loop หาผลรวม 1..100 และระบุ state ที่เปลี่ยนในแต่ละรอบ
3. เขียน function min_of_two
4. สร้าง struct Point {x,y} และ function พิมพ์ค่า
5. วาด call chain ของ factorial(4)

## Intermediate
6. เขียน C function swap(int*, int*) และอธิบายทุก dereference
7. เขียน C++ function swap_ref(int&, int&)
8. อธิบาย scope กับ lifetime ว่าต่างกันอย่างไร
9. หา bug ใน function ที่ return address ของ local variable
10. เขียน recursive sum 1..n พร้อม base case และ precondition

## Advanced
11. เปรียบเทียบ pass-by-value กับ indirection สำหรับ struct ขนาดใหญ่
12. ออกแบบ struct Range ที่รักษา invariant begin <= end
13. อธิบายว่า pointer สามารถชี้ stack object ได้หรือไม่และมีเงื่อนไขอะไร
14. สร้าง template function clamp(T value,T low,T high)
15. อธิบายว่าการใช้ global mutable state ทำให้ reasoning ยากขึ้นอย่างไร

## Implementation
16. เขียน C program เก็บ array ของ Point 100 ตัวและหาจุดที่ x มากที่สุด
17. เขียน C++ generic Box<T> ที่ get/set ค่าได้
18. เขียน recursive binary representation printer ของ unsigned integer

## Challenge / Research
19. ศึกษาคำว่า strict aliasing ใน C/C++ แล้วสรุปว่าทำไม type-punning แบบผิดกฎอาจทำให้ optimization เปลี่ยนผล
20. เปรียบเทียบ ownership model ของ C, Rust และ Java ในโจทย์สร้าง linked node โดยยังไม่ต้อง implement linked list
