# Implementation Notes

แต่ละ `ArenaBlock` มี raw storage พร้อม extra 4095 bytes แล้ว align data base ไปที่ 4096-byte boundary.

ถ้า head block ไม่พอ จะสร้าง block ใหม่ที่ capacity = max(default_block_size, size + alignment - 1). ถ้า placement ภายใน block ใหม่ล้มเหลวผิดคาด implementation rollback block ที่เพิ่ง publish.

Arena ไม่ thread-safe; caller ต้อง synchronize เอง.
