# Pitfalls — Range Tree

- calling a Segment Tree a Range Tree
- storing only one global y-sorted array
- rebuilding each associated list with O(m log m) sort and claiming optimal Theta(n log n) construction
- mishandling duplicate x coordinates
- mixing closed and half-open rectangle boundaries
- forgetting output term k in reporting complexity
- claiming O(n) memory for a classic 2D Range Tree
- mutating input points after static build and expecting the index to update
