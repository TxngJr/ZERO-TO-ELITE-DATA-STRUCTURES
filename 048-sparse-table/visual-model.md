# Visual Model — Sparse Table

Array length 8.

Level 0, blocks of 1:
    [0] [1] [2] [3] [4] [5] [6] [7]

Level 1, blocks of 2:
    [0..2) [1..3) [2..4) ...

Level 2, blocks of 4:
    [0..4) [1..5) [2..6) ...

Level 3, blocks of 8:
    [0..8)

Query [2,7), length 5.

k=2, block=4:

    left  [2,6)
    right [3,7)

Overlap [3,6) is harmless for min.
