# Visual Model — Quadtree

Parent rectangle:

    +-----------+-----------+
    |    NW     |    NE     |
    |           |           |
    +-----------+-----------+
    |    SW     |    SE     |
    |           |           |
    +-----------+-----------+

Each occupied child may split again.

A query rectangle only descends into intersecting quadrants.
