# Exercises

## Beginner
1. อธิบาย immutable object ด้วยตัวอย่างที่ไม่ใช่ collection.
2. Persistent และ immutable ต่างกันอย่างไร?
3. ทำไม FrozenIntSet ไม่ต้องมี tombstone?
4. คำนวณ load factor เมื่อ size=200, capacity=512.
5. อธิบายเหตุผลของ power-of-two capacity.

## Intermediate
6. เพิ่ม iterator ที่ visit occupied keys.
7. เพิ่ม constructor จาก sorted unique input และเปรียบเทียบ.
8. เขียน property ว่าทุก input key ต้อง contains=true.
9. วัด probe count distribution ที่ load 0.25/0.5/0.75.
10. อธิบาย publication pattern “build then atomic swap” ในเชิงแนวคิด.

## Advanced
11. ออกแบบ immutable ordered set.
12. เปรียบเทียบ FrozenIntSet กับ perfect hashing.
13. วิเคราะห์ peak memory เมื่อ rebuild snapshot ขนาด 2 GiB.
14. ออกแบบ string keys พร้อม ownership ที่ไม่ dangling.
15. อธิบาย logical immutability กับ internal memoization.

## Implementation
16. เพิ่ม `FrozenIntMap<int,int>`.
17. เพิ่ม probe-statistics benchmark.
18. ทำ serialization + validation เมื่อ load.

## Challenge / Research
19. เปรียบเทียบ HAMT, perfect hash และ frozen linear-probing set สำหรับ read-mostly workload.
20. ออกแบบ reclamation scheme สำหรับหลาย reader ที่ถือ old snapshots.
