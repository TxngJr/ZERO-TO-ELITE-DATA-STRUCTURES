# Common Mistakes

- ใช้ size class เล็กกว่า request.
- นับ reserved bytes เป็น internal fragmentation ทั้งหมด.
- ไม่เก็บ requested size แล้ววัด fragmentation ผิด.
- free pointer กลาง slot.
- double free แล้ว free stack มี duplicate index.
- reclaim slab ที่ยังมี live object.
- class metadata overflow แล้ว publish allocation ไปก่อน.
- คิดว่า teaching linear slab search เท่ากับ production slab allocator.
