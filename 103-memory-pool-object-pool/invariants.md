# Invariants

1. object_size > 0.
2. stride >= object_size และ aligned for max_align_t.
3. capacity > 0.
4. free_top <= capacity.
5. in_use_count <= capacity.
6. free_top + in_use_count = capacity.
7. ทุก free-stack index อยู่ใน range.
8. ไม่มี duplicate index ใน active free stack.
9. free-stack slots ต้องมี in_use=0.
10. release รับเฉพาะ exact slot-start pointer.
