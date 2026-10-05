# Pitfalls — Quadtree

- using inclusive bounds inconsistently
- midpoint overflow
- splitting a unit-width/height integer cell
- infinite subdivision for coincident points
- losing points during redistribution
- allocating children then failing halfway without rollback
- assuming balanced logarithmic height
- forgetting subtree_size updates
