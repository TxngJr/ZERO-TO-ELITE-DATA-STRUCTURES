# Common Mistakes

- ใช้ marker หลัง reset.
- align offset โดยไม่เช็ก overflow.
- รองรับ over-alignment แต่จอง slack ไม่พอ.
- ลืมนับ alignment padding ใน used bytes.
- publish block ใหม่แล้วไม่ rollback เมื่อ failure.
- reset แล้ว client ยังใช้ pointer เก่า.
- ใช้ arena กับ lifetime ที่กระจัดกระจายโดยไม่มี phase boundaries.
