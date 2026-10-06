# Implementation Notes

Allocator จอง managed storage ครั้งเดียว แต่ metadata extents ใช้ system heap เพื่อทำให้ split/coalesce logic มองเห็นง่าย.

Allocation list เก็บ exact start/size ของ live allocations. Release ต้อง match exact start pointer; pointer กลาง block ถูก reject.

Address arithmetic ใช้ `uintptr_t` เพื่อคำนวณ alignment โดยไม่ทำ relational comparison ของ unrelated C pointers.

Design นี้ยังไม่ production allocator:
- first-fit เป็น linear scan
- allocation lookup ตอน free เป็น linear scan
- metadata ใช้ malloc/free
