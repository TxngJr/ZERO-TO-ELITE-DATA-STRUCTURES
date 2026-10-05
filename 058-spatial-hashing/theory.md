# Theory — Spatial Hashing

Spatial hashing converts continuous/integer coordinates into discrete cell coordinates, then maps those coordinates through a hash table.

The grid gives locality; hashing removes the need to allocate the whole grid.

Correctness has two layers:
1. every point must be assigned to exactly its floor-divided cell
2. queries enumerate every cell intersecting the rectangle, then exact-filter points
