# Visual Model — B* Redistribution

Suppose order m=6, max keys=5.

Parent:

          [50]
         /    \
 [10 20 30 40]   [60 70 80 90 100]

Right child overflows after inserting 110:

 [60 70 80 90 100 110]

Left sibling still has room.

Combine:

 10 20 30 40 | 50 | 60 70 80 90 100 110

Redistribute around a new separator:

 left            parent      right
[10 20 30 40 50]  [60]   [70 80 90 100 110]

No new node needed.

## 2-to-3 concept

Two full/overflowing siblings plus parent separator become:

    node A | sep1 | node B | sep2 | node C

instead of one ordinary 1-to-2 split.
