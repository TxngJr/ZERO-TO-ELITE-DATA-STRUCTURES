# Invariants — IntSpatialHash

1. cell_size > 0
2. table capacity is a power of two
3. every occupied cell has one unique (cell_x,cell_y) key
4. every cell is reachable from exactly one hash bucket chain
5. every point maps back to its owning cell via floor_div
6. total cell point counts equal tree.size
7. cell_count equals number of arena cells
