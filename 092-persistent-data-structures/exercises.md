# Exercises

## Beginner
1. วาด versions หลัง insert 10,5,20,7.
2. ระบุ nodes ที่ share ระหว่างสอง versions.
3. อธิบาย persistence ต่างจาก backup file อย่างไร.
4. แยก partial vs full persistence.
5. คำนวณ nodes ใหม่เมื่อ update path ยาว 6.

## Intermediate
6. เพิ่ม inorder visitor โดยไม่ mutate tree.
7. เพิ่ม min/max query.
8. เขียน test ว่า old root ไม่เปลี่ยนหลัง erase.
9. วิเคราะห์ no-op insert ว่าทำไมไม่ควร allocate.
10. เปรียบเทียบ arena กับ reference counting.

## Advanced
11. เปลี่ยน BST เป็น AVL path-copying.
12. ออกแบบ version ID table และ parent links.
13. เพิ่ม snapshot deletion พร้อม reclamation design.
14. วิเคราะห์ memory growth สำหรับ 1M updates balanced tree.
15. อธิบาย confluence และเหตุผลที่ API ปัจจุบันยังไม่รองรับ merge.

## Implementation
16. เพิ่ม persistent rank/select ด้วย subtree sizes.
17. เพิ่ม randomized branching test ที่ update version เก่าโดยตรง.
18. ทำ benchmark full-copy vs path-copy.

## Challenge / Research
19. ศึกษา node-copying persistence technique และเปรียบเทียบ path copying.
20. ออกแบบ persistent B+ Tree pages สำหรับ storage engine พร้อม crash/recovery questions.
