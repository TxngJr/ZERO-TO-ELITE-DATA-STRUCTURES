# Lab — Split, Fragment, Coalesce

1. Create 1 MiB allocator.
2. Run 50,000 random allocate/free operations.
3. Requests 1..1024 bytes.
4. Vary alignment 1..64.
5. Track independent live bytes/count.
6. Periodically validate all extents.
7. Free everything.
8. Confirm one free block exactly capacity.

ลองสร้าง workload ที่ free ทุก allocation สลับตัว แล้วสังเกต total-free เทียบ largest-free เพื่อเห็น external fragmentation.
