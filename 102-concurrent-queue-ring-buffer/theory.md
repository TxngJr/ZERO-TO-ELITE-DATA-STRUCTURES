# Theory — MPMC Ring Coordination

Modulo index บอก physical slot แต่ไม่บอก generation. เมื่อ head/tail wrap, slot 3 อาจหมายถึงคนละ logical position. Sequence number ต่อ slot ทำหน้าที่เป็น generation/ownership state.

Producer publish ordering:
`data write -> release sequence store`.

Consumer:
`acquire sequence load -> data read`.

หลัง consumer อ่านเสร็จ sequence ถูก advance ด้วย capacity เพื่อบอก producer รุ่นถัดไปว่า slot reusable.

Correctness และ progress แยกกัน: sequence protocol อาจ data-race-free/linearizable ภายใต้ algorithm contract แต่ไม่ได้ทำให้ formal lock-free claim เกิดขึ้นเอง.
