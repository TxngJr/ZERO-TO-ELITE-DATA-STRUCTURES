# Implementation Notes

Creation computes:
1. max(rows, cols)
2. next power of two `side`
3. `side * side` padded elements with overflow checks

`morton_index(row,col)` interleaves coordinate bits. Since side is a power of two, every logical coordinate maps into `[0, side²)`.

Physical Z-order sum walks storage linearly; padding is required to remain zero so it does not change logical aggregates.
