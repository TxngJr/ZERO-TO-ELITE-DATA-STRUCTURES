# Invariants

1. leaf_count > 0 and leaf_size > 0.
2. level 0 count equals leaf_count.
3. each next level count = ceil(previous/2).
4. final level count = 1.
5. every internal hash equals H(0x01 || left || right).
6. odd final child pairs duplicate the last hash.
7. root equals the sole hash in final level.
8. proof step count = level_count - 1.
9. each proof step records one 32-byte sibling plus side.
10. verification accepts only when folded hash equals supplied root.
