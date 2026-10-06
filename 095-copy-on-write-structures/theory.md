# Theory

Naive clone copy Θ(n) ทันที. COW clone เป็น Θ(1) แล้วเลื่อน copy ไปตอน first write.

Semantic layer เห็น vectors แยกกัน แม้ storage layer share bytes. Correctness requirement คือ mutation ของ y ห้ามเปลี่ยนค่าที่อ่านจาก x.

`refs=1` หมายถึง unique storage; `refs>1` ต้อง detach ก่อน write. Atomic refcount สามารถแก้ race บน counter ได้บางส่วน แต่ไม่ได้ทำให้ data mutation ของ vector thread-safe โดยอัตโนมัติ.
