# Common Mistakes

- split แล้วทำ prefix/suffix bytes หาย.
- ไม่ coalesce adjacent blocks.
- align start แต่ลืมหัก prefix จาก available size.
- mutate block ก่อน metadata allocation สำเร็จ.
- free pointer กลาง allocation.
- double free แล้ว free extents overlap.
- ใช้ total free bytes เป็นคำตอบว่า allocation ใหญ่ทำได้หรือไม่.
- ไม่เช็ก overflow ตอน round/alignment arithmetic.
