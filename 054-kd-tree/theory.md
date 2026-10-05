# Theory — KD-Tree

## Space partitioning

A KD-Tree recursively splits point space by coordinate hyperplanes.

Unlike a normal BST, the ordering dimension changes with depth.

## Balanced by count

Median selection makes child subtree sizes differ by at most one at construction time.

Thus static height is O(log n), even when many coordinate values are equal.

## Range pruning

A subtree bounding box is a conservative spatial summary:
- disjoint box -> impossible to contain an answer
- fully covered box -> every subtree point is an answer
- partial overlap -> descend

## Nearest pruning

Bounding-box distance is an admissible lower bound.

If:

    lower_bound > best_distance

then the subtree cannot improve the current best and is safe to skip.

Worst case remains O(n), such as unfavorable high-dimensional/distribution/query situations.
