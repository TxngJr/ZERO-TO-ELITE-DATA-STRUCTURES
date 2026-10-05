# Exercises — Chapter 014

## Beginner
1. วาด table 4 buckets + collisions
2. lookup ทำกี่ขั้นหลัก
3. update key เดิมเปลี่ยน size หรือไม่
4. load factor คืออะไร
5. resize ทำไมต้อง rehash

## Intermediate
6. implement contains
7. อธิบาย pointer-to-pointer removal
8. trace rehash 8→16
9. วิเคราะห์ expected chain length
10. อธิบาย worst collision case

## Advanced
11. พิสูจน์ geometric resize amortization
12. วิเคราะห์ threshold 0.5 vs 2.0 chaining trade-off
13. อธิบาย cached hash field
14. ออกแบบ iterator contract ที่ไม่ promise order
15. อธิบาย failure atomicity ของ resize

## Implementation
16. เพิ่ม clear
17. เพิ่ม reserve(expected_entries)
18. เพิ่ม bucket statistics API

## Challenge / Research
19. ศึกษา treeified buckets concept ในบาง runtimes
20. เปรียบเทียบ separate chaining กับ SwissTable-style open addressing ในระดับ motivation/cache behavior
