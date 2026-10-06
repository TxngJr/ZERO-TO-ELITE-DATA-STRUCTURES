# Theory — Slab Allocation

Slab allocation ใช้ repeated fixed-size caches/classes เพื่อจัด objects หลายขนาดโดยลด fragmentation และ amortize allocator metadata.

คำสำคัญ:
- size class
- slab/page
- slot/object
- partial/full/empty slab
- internal fragmentation
- slab retention/reclaim

Chapter 103 มี pool เดียวสำหรับ object size เดียว. Chapter 105 มีหลาย classes และสร้างหลาย slabs ต่อ class ตาม demand.

Production slab allocators อาจมี per-CPU caches, object constructors, coloring, NUMA policy และ fast partial-slab lists; teaching implementation นี้ยังไม่อ้าง feature เหล่านั้น.
