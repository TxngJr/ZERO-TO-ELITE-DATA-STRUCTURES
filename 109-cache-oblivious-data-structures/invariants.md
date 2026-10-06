# Invariants

1. rows, cols > 0.
2. padded side is power of two.
3. padded side >= max(rows, cols).
4. total elements = side² without overflow.
5. every logical coordinate maps to one unique Morton index.
6. physical storage follows Morton/Z-order.
7. padded coordinates outside logical rows/cols remain zero.
8. logical copy/transpose never expose padding.
