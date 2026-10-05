# Visual Model — 2D KD-Tree

Depth 0: split x

               (5,4)  x
              /          \
Depth 1 y  (2,3)        (8,7)
          /   \          /   \
Depth 2 x ...

Each subtree also owns a bounding box:

    [min_x,max_x] x [min_y,max_y]

Range query:
    skip boxes outside rectangle

Nearest query:
    skip boxes farther than current best
