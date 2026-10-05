# Implementation — IntQuadtree

Point:
- x
- y
- id

Node:
- bounds
- subtree_size
- dynamic leaf point bucket
- children[4]

Tree:
- root
- bucket_capacity
- max_depth
- total size
- node count

Insert redistributes an overflowing leaf only when a valid split is possible.
