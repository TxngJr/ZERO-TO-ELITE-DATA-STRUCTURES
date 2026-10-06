# Invariants

1. `capacity` เป็น power of two และอย่างน้อย 8.
2. `size <= capacity/2`.
3. `used[i]` ต้องเป็น 0 หรือ 1.
4. จำนวน occupied slots เท่ากับ `size`.
5. ทุก occupied key ต้องค้นเจอด้วย probe algorithm ปกติ.
6. ไม่มี public mutation API.
7. input ownership ไม่ถูกยืมหลัง constructor; keys ถูก copy แล้ว.

Invariant ข้อ 5 สำคัญเพราะ key ที่วางผิด probe cluster อาจ “อยู่ใน array” แต่ query หาไม่เจอ.
