# Implementation Notes

แต่ละ slab มี free stack แยกจาก payload และ byte in-use metadata. Requested size ต่อ live slot เก็บเป็น `unsigned short` เพราะ max request = 256.

Pointer release แปลง addresses เป็น `uintptr_t` เพื่อหา owning slab และตรวจว่า pointer อยู่ตรง slot boundary.

Allocation metrics ถูกตรวจ overflow ก่อน publish live slot. Empty slabs ถูก retain จน caller เรียก `slab_trim_empty`.

Implementation ไม่ thread-safe.
