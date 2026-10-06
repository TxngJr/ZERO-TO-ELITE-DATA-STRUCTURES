# Common Mistakes

- ไม่ round stride แล้ว slot ถัดไป misaligned.
- ใช้ payload เป็น free-list pointerทั้งที่ object เล็กกว่า pointer.
- ยอมให้ pointer กลาง slot ถูก free.
- ไม่ตรวจ double free.
- คูณ stride*capacity โดยไม่เช็ก overflow.
- คิดว่า pool ขนาดคงที่แก้ internal fragmentation ทั้งหมด.
- destroy pool ขณะที่ client ยังถือ live pointers.
