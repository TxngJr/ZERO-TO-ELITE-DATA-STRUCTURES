# Common Mistakes

## นับ rank แบบ inclusive โดยไม่ตั้งใจ
**Bug:** caller คิดว่า `rank1(i)` รวม bit `i`.
**Cause:** contract ไม่ชัด.
**Fix:** ใช้ half-open `[0,end)` ตลอดและ test `end=0`, `end=n`.

## Padding ปลอมใน select0
**Bug:** complement word สุดท้ายแล้วเห็น zeros หลัง logical end.
**Detect:** test sizes 63/64/65.
**Fix:** mask valid bits ก่อน popcount/select.

## เรียก practical directory ว่า strict succinct
**Bug:** อ้าง `n+o(n)` ทั้งที่ metadata interval คงที่.
**Fix:** แยก theorem จาก implementation accounting.

## Overflow ตอน ceil division
ตรวจ `nbits + 63` ก่อนคำนวณ word count.

## Benchmark เฉพาะ payload
ต้องรายงาน directory และ object metadata ด้วย ไม่ใช่แค่ `ceil(n/8)` bytes.
