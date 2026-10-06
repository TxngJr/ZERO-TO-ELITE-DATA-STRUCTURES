# Invariants

1. size classes = 16/32/64/128/256.
2. every slab belongs to exactly one class.
3. slab class_size ตรง owning class.
4. free_top + live = slots.
5. live slot requested_size อยู่ใน 1..class_size.
6. free-stack indices valid/non-duplicate.
7. free-stack slots ต้อง in_use=0.
8. live_objects = sum slab.live.
9. requested_live_bytes = sum requested sizes.
10. allocated_class_bytes = sum class_size of live slots.
11. reserved_payload_bytes = sum class_size*slots of all slabs.
