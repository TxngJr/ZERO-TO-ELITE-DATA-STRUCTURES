# Common Mistakes

- ใช้ raw slot เป็น entity ID แล้ว stale references ชี้ entity ใหม่.
- sparse index ไม่ update หลัง swap-remove.
- destroy entity แล้วลืม remove component.
- grid query หลัง world mutate โดยไม่ rebuild.
- ใส่ entity ในหลาย cells ทั้งที่ model นี้ index point position เดียว.
- ใช้ cell hit เป็น exact resultโดยไม่ filter coordinate.
- ลืม bounds/overflow ตอนคำนวณ cell count.
