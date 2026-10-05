# Implementation — IntSpatialHash

Hash table:
- bucket_heads[capacity]
- separate chaining

Cell arena:
- cell_x
- cell_y
- next index
- dynamic point array

Indices remain valid if the cell arena reallocates.

Rehash only rebuilds bucket-head/next relationships.
