# Theory — Pool Allocation

General-purpose allocators รองรับหลาย sizes/lifetimes จึงมี metadata/search/coalescing complexity. Object pool แลก flexibility กับ predictable fixed-size reuse.

Pool ช่วย:
- ลด allocation metadata per object
- ลด external fragmentation ภายใน pool
- ทำ allocation/release เป็น constant-time stack operations
- เพิ่ม locality เมื่อ objects อยู่ใน contiguous storage

แต่ pool อาจ waste memory เมื่อ capacity สูงกว่าความต้องการจริง และไม่เหมาะกับ object sizes ที่กระจายมาก.

## Internal Padding

object_size อาจถูก round เป็น stride เพื่อ alignment:

`internal padding per slot = stride - object_size`.

ดังนั้น fixed pool ยังมี internal fragmentation ได้ แม้ไม่มี external fragmentation ภายใน storage.
