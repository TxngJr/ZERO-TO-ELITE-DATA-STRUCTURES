# Exercises — Chapter 002

## Beginner
1. อธิบาย virtual address กับ physical address ว่าทำไมไม่ควรถือว่าเหมือนกัน
2. วาด local int และ pointer ที่ชี้มัน
3. เขียน malloc int 100 ตัวและ free อย่างถูกต้อง
4. อธิบาย memory leak ด้วยตัวอย่าง
5. อธิบาย dangling pointer

## Intermediate
6. หา bug จาก double free
7. หา bug จาก use-after-free
8. อธิบายว่าทำไม return &local จึงผิด
9. เขียน safe realloc pattern ด้วย temporary pointer
10. เปรียบเทียบ contiguous array กับ linked nodes ในแง่ locality

## Advanced
11. อธิบาย temporal vs spatial locality
12. ทดลอง sizeof struct ที่มี char/int/double สลับลำดับและบันทึกผล
13. อธิบายว่าทำไม optimizer อาจทำให้ local variable ไม่ปรากฏใน stack memory แบบที่คาด
14. อธิบาย pointer chasing dependency chain
15. วิเคราะห์ ownership ของ buffer ที่ function allocate แล้วคืนให้ caller

## Implementation
16. เขียน dynamic integer buffer ที่มี create/destroy
17. เพิ่ม checked write ที่ไม่ยอม index เกินขอบเขต
18. เขียน program เปรียบเทียบ row-major กับ column-major traversal

## Challenge / Research
19. ศึกษา ASLR บน Linux แล้วสรุปว่ามีเป้าหมายอะไร
20. ศึกษา cache line concept ของ CPU เครื่องตนเองผ่าน lscpu/sysfs/documentation แล้วอธิบายว่ามันอาจมีผลต่อ array traversal อย่างไร
