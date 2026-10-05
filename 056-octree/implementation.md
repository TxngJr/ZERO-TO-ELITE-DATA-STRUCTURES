# Implementation — IntOctree

Point:
- x,y,z,id

Node:
- 3D half-open bounds
- subtree_size
- dynamic leaf bucket
- children[8]

Tree:
- root
- total point count
- allocated node count
- bucket capacity
- max depth

Midpoint and subdivision guards mirror Chapter 055.
