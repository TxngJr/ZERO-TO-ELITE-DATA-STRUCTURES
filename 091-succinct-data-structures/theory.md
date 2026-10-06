# Theory — Succinct Data Structures

## Compact, Compressed, Succinct

สามคำนี้ไม่ใช่คำเดียวกัน:

- **Compact**: ใช้ memory น้อยในเชิง engineering แต่ไม่มี bound ทางทฤษฎีชัดเจน
- **Compressed**: exploit redundancy เพื่อให้ representation เล็กลง; อาจต้อง decode ก่อน query
- **Succinct**: ใช้พื้นที่ใกล้ information-theoretic lower bound พร้อมรองรับ operations ที่กำหนด

สำหรับ arbitrary bit vector ยาว `n`, payload ดิบต้องการ `n` bits อยู่แล้ว. ความท้าทายจึงไม่ใช่บีบ payload ต่ำกว่า `n` โดยทั่วไป แต่คือเพิ่ม index metadata เพียง `o(n)` bits เพื่อให้ query เร็ว.

## Rank / Select as Navigation Primitives

`rank` แปลง position -> occurrence count.
`select` แปลง occurrence count -> position.

สอง operation นี้ทำหน้าที่เหมือน coordinate transforms และใช้สร้าง navigation ของ topology ที่ encode ด้วย bits ได้ เช่น balanced-parentheses tree representation.

## Directory Trade-off

Implementation ของบทใช้ 512-bit superblocks:

```text
8 x uint64_t payload words
1 x prefix checkpoint
```

ข้อดีคือเข้าใจง่ายและ bounded local scan. ข้อเสียคือ metadata ratio เป็นค่าคงที่ ไม่ได้หายไป asymptotically. นี่คือเหตุผลที่เอกสารไม่เรียก implementation นี้ว่า strict Jacobson-style `n+o(n)` solution.

## Static Assumption

Structure นี้เป็น static หลัง build. ถ้า bit เปลี่ยน ค่า prefix หลังตำแหน่งนั้นอาจต้องแก้จำนวนมาก. Dynamic succinct structures เป็นหัวข้อยากกว่าเพราะต้องรักษาทั้ง compression bound และ query/update guarantees พร้อมกัน.
