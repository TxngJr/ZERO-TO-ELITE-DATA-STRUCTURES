# Implementation Notes

`PersistentIntSet` ใช้ immutable BST nodes และ block arena.

## Transactional Allocation

Top-level insert/erase บันทึก arena mark ก่อนเริ่ม. ถ้า allocation ระหว่าง recursion ล้มเหลว:

1. free blocks ที่สร้างหลัง mark
2. reset `used` ของ block เดิม
3. restore node/block counters
4. ไม่ publish `out_root`

ดังนั้น caller ไม่ได้รับ half-built version.

## Stable Addresses

ห้ามเก็บ nodes ใน growable flat array ที่ `realloc` ได้ เพราะ pointer จาก historical roots จะ invalid เมื่อ storage ย้าย. Block allocation ทำให้ address ของ node ที่สร้างแล้วไม่เปลี่ยน.

## No-op Updates

Duplicate insert และ missing erase คืน root เดิมโดยไม่สร้าง path copies. Tests ตรวจ `arena_allocated_nodes` ว่าไม่เพิ่มใน no-op case.
