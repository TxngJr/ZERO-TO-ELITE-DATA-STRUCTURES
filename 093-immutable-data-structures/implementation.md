# Implementation Notes

Constructor:

1. validate input
2. choose power-of-two capacity at least roughly `2 * count`
3. allocate `keys[]` and zeroed `used[]`
4. hash each input value
5. linear probe
6. insert first occurrence; duplicate stops at equal key

Hash mixer รับ signed int โดย sign-extend เป็น 64-bit แล้ว mix. Index ใช้ bit mask เพราะ capacity เป็น power of two.

ไม่มี tombstone เพราะไม่มี deletion. นี่เป็นตัวอย่างสำคัญว่าการรู้ workload/semantic constraints ช่วย simplify representation ได้อย่างไร.
