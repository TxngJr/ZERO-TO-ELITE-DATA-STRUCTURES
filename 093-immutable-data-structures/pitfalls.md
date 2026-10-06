# Common Mistakes

## เรียก const pointer ว่า immutable
`const` ใน C จำกัดผ่าน pointer นั้น แต่ไม่ได้พิสูจน์ว่าไม่มี alias อื่น mutate object. API/ownership design ต้องร่วมกันบังคับ semantics.

## ใช้ tombstone ทั้งที่ไม่มี deletion
เพิ่ม metadata และ branch โดยไม่จำเป็น.

## Capacity ไม่เป็น power of twoแต่ใช้ `& (capacity-1)`
ทำให้ index distribution ผิด contract. Validator ตรวจ power-of-two.

## Hash signed int โดย cast แบบไม่ตั้งใจ
ต้องกำหนด mapping ให้ deterministic; implementation sign-extend ก่อน mix.

## อ้าง thread-safe โดยไม่พูด lifetime/publication
Immutable state ลด write races แต่ free/reclamation และ publication ยังเป็น concurrency concern.

## Rebuild ทุก update โดยไม่วัด
Immutable snapshot design เหมาะเมื่อ read-many; write-heavy workload อาจแพงกว่า mutable table.
