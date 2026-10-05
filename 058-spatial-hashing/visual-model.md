# Visual Model — Sparse Grid Hash

Cell size = 10.

x coordinates:

    ... [-20,-10) [-10,0) [0,10) [10,20) ...

cell_x:

    ...    -2        -1       0       1

Only occupied cells exist in the hash table.

Query [3,24) touches x cells:

    0,1,2

Boundary cells are scanned, then points are exact-filtered.
