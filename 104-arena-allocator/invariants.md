# Invariants

1. default_block_size > 0.
2. arena มี base block หลัง create/reset.
3. block used <= capacity.
4. block data base aligned 4096.
5. total_used = sum(block.used).
6. reserved_bytes = sum(block.capacity).
7. block_count ตรง linked-list length.
8. alignment ต้องเป็น power-of-two <= 4096.
9. mark ใช้ได้เฉพาะ generation ปัจจุบัน.
10. reset target block ต้องยังอยู่ใน arena.
