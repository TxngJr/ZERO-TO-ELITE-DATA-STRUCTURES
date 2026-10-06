# Invariants

1. rows, cols, tile dimensions > 0.
2. tile counts = ceil(logical dimensions / tile dimensions).
3. tile_elems = tile_rows * tile_cols.
4. total_elems = total_tiles * tile_elems.
5. every logical cell maps to exactly one valid physical offset.
6. tiles are contiguous in storage.
7. cells outside logical edge inside padded tiles remain zero.
8. copy/transpose never expose padding as logical data.
9. all layout multiplications are overflow checked at creation.
