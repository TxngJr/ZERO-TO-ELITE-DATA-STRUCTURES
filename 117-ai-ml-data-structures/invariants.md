# Invariants

Dataset:
1. rows/features > 0.
2. backing feature matrix has rows×features elements.
3. labels and initialized arrays have one entry per row.
4. row view/gather only expose initialized rows.

Batch plan:
5. order length equals rows.
6. order is a permutation of [0, rows).
7. cursor <= rows.
8. batch_size > 0.

Embedding:
9. rows/dims > 0.
10. each initialized embedding row has exactly dims contiguous floats.
