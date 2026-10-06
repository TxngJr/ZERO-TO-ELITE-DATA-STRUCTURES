# Invariants

1. block capacity > 0.
2. values are globally strictly increasing.
3. block count = ceil(count/block_capacity).
4. every block start/count stays inside values array.
5. each block first/last matches its physical endpoints.
6. directory block ranges are strictly ordered and non-overlapping.
7. every value belongs to exactly one block.
8. I/O counters never affect logical contents.
