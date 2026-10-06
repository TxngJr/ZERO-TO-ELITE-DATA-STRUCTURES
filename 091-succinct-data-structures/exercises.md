# Exercises

## Beginner
1. คำนวณ `rank1` ของ bit vector 16 bits ด้วยมือ 5 ตำแหน่ง.
2. อธิบายความต่างของ `rank1(i)` กับ `select1(k)`.
3. หา word index และ bit offset ของ positions 0, 63, 64, 129.
4. อธิบายเหตุผลที่ final padding ต้องเป็น zero.
5. วาด superblocks สำหรับ 1,100 bits.

## Intermediate
6. เปลี่ยน superblock จาก 8 เป็น 4 words แล้วคำนวณ metadata overhead.
7. เพิ่ม API `count1(begin,end)` โดยใช้ rank.
8. เขียน property `rank1(i+1)-rank1(i) == bit[i]`.
9. พิสูจน์ว่า `rank0(i)+rank1(i)=i`.
10. วิเคราะห์ select เมื่อ vector มี ones น้อยมาก.

## Advanced
11. ออกแบบ two-level rank directory.
12. เปรียบเทียบ binary-search superblocks กับ sampled select directory.
13. วิเคราะห์ cache footprint เมื่อ vector มี 1 GiB.
14. ศึกษา broadword select และอธิบายแนวคิดโดยไม่ copy implementation.
15. อธิบายว่าทำไม dynamic updates ทำให้ succinct indexing ยากขึ้น.

## Implementation
16. เพิ่ม `select1` acceleration checkpoints ทุก 1,024 ones.
17. เพิ่ม serialization format ที่ validate padding/directory เมื่อ load.
18. สร้าง property-based randomized test 100,000 queries.

## Challenge / Research
19. ออกแบบ parameter ที่ทำให้ metadata ratio ลดเมื่อ `n` โต และวิเคราะห์ query cost.
20. อ่านงาน classic rank/select แล้วเขียน comparison ระหว่าง theorem กับ implementation ของบทนี้.
