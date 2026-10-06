# Theory — Concurrent Resizable Hash Maps

Concurrent hash map ต้องแก้สองปัญหาคนละระดับ:

- local synchronization ของ bucket/node
- global synchronization ของ table shape ระหว่าง resize

Lock striping อย่างเดียวเหมาะเมื่อ table shape คงที่. เมื่อ rehash เกิดขึ้น key อาจย้าย bucket จึงต้องมี protocol ที่ทำให้ lookup ไม่อ่าน old/new table แบบครึ่งกลาง.

Implementation นี้เลือก coarse resize barrier ด้วย RW-lock เพื่อให้ proof ง่าย. Production designs อาจใช้ incremental migration, forwarding nodes หรือ lock-free techniques แต่ complexity สูงขึ้นมาก.
