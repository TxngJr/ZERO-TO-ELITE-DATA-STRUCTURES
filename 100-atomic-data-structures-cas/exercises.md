# Exercises

## Beginner
1. หา word/mask ของ bit 130.
2. อธิบาย fetch_or return ค่าอะไร.
3. test-and-clear ใช้ operation ใด.
4. strong CAS ต่างจาก weak CAS อย่างไร.
5. atomic load เป็น RMW หรือไม่.

## Intermediate
6. เขียน atomic test-and-set ด้วย CAS loop.
7. อธิบาย linearization point ของ fetch_or.
8. เพิ่ม range-set ที่เป็น word-aligned.
9. วัด contention เมื่อทุก thread toggle bit เดียว.
10. อธิบาย acquire/release publication.

## Advanced
11. สร้าง ABA timeline.
12. เปรียบเทียบ 16/32/64-bit version tag.
13. วิเคราะห์ false sharing ระหว่าง adjacent words.
14. เพิ่ม exponential backoff ให้ CAS loop แล้ว benchmark.
15. อธิบาย why atomic_is_lock_free matters.

## Implementation
16. เพิ่ม atomic minimum.
17. เพิ่ม compare-and-swap bit range API.
18. เพิ่ม CAS retry histogram.

## Challenge / Research
19. เปรียบเทียบ LL/SC กับ CAS conceptually.
20. ออกแบบ tagged pointer layout โดยคำนึง alignment/address bits.
