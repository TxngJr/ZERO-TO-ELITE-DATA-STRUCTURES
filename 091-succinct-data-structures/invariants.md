# Invariants

1. `word_count = ceil(nbits / 64)`.
2. `super_count = ceil(word_count / 8)` เมื่อ non-empty.
3. `super_rank[0] = 0`.
4. `super_rank[s]` เท่ากับ ones ก่อน word `8*s`.
5. `super_rank[super_count] = ones`.
6. padding bits หลัง `nbits` ใน word สุดท้ายเป็น zero.
7. ทุก query ห้ามคืน index `>= nbits`.

Validator recompute popcounts จาก payload แล้วเทียบ directory เพื่อจับ metadata corruption.
