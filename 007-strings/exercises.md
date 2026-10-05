# Exercises — Chapter 007

## Beginner
1. วาด bytes ของ "cat" แบบ C string
2. length และ storage bytes ต่างกันอย่างไร
3. อธิบาย strlen complexity
4. mutable vs immutable ต่างกันอย่างไร
5. ทำไม UTF-8 character อาจมากกว่า 1 byte

## Intermediate
6. ออกแบบ length-aware string struct
7. เขียน append_char contract
8. วิเคราะห์ insert ที่ตำแหน่งกลาง
9. อธิบาย string view lifetime
10. อธิบาย embedded null

## Advanced
11. derive repeated immutable concatenation Θ(n²)
12. อธิบาย aliasing problem ใน self-append
13. เปรียบเทียบ null-terminated กับ length-prefixed
14. อธิบาย interning trade-offs
15. อธิบาย small-string optimization เป็น concept

## Implementation
16. เพิ่ม starts_with
17. เพิ่ม ends_with
18. เพิ่ม replace-first แบบ byte-oriented

## Challenge / Research
19. ศึกษา UTF-8 indexing complexity และเหตุผลที่ code-point index ไม่ใช่ trivial byte offset
20. เปรียบเทียบ C++ std::string, Rust String และ Java String semantics โดยแยก API guarantee กับ implementation detail
