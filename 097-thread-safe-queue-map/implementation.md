# Implementation

Queue ใช้ circular array, one mutex, `not_empty`, `not_full` และ `closed`.

Map ใช้ fixed bucket array. แต่ละ bucket มี mutex + singly linked list. `size` ถูกป้องกันด้วย mutex แยก.

`tsm_validate_quiescent` มี contract ว่าต้องไม่มี concurrent users และจงใจ lock ทีละ bucket ไม่ถือทุก bucket พร้อมกัน. วิธีนี้ลด artificial many-lock critical section และหลีกเลี่ยงการทำ validator ให้เป็นตัวสร้าง contention เอง.

ไม่มี dynamic rehash ในบทนี้ เพราะ rehash ข้ามหลาย buckets ต้องมี protocol ที่ซับซ้อนกว่าและจะไปเชื่อม Chapter 101 Concurrent Hash Map.
