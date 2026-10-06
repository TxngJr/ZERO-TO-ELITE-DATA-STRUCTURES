# Quiz

1. Succinct ต่างจาก generic compression อย่างไร?
2. `rank1(0)` ต้องเท่ากับเท่าไร?
3. Half-open prefix ช่วยลด off-by-one อย่างไร?
4. `select1(0)` หมายถึงอะไร?
5. ทำไม padding ของ word สุดท้ายมีผลกับ `select0`?
6. Directory checkpoint ช่วย rank อย่างไร?
7. ทำไม fixed 512-bit checkpoint ไม่พิสูจน์ `o(n)` metadata?
8. `rank0(i)` derive จาก `rank1(i)` อย่างไร?
9. ถ้า vector เป็น static เราได้ประโยชน์อะไร?
10. ถ้าแก้ bit หนึ่งตัว directory ส่วนใดอาจ stale?
11. ทำไม benchmark ต้องนับ metadata?
12. Rank/select เชื่อมกับ succinct trees ได้อย่างไร?
13. Worst-case ของ select ใน implementation นี้มาจากขั้นตอนไหน?
14. Boundary ใดจับ bugs ของ word packing ได้ดี?
15. เมื่อใด plain bitset เหมาะกว่าการเพิ่ม succinct-style index?
