# Pitfalls — KD-Tree

- assuming one global BST key
- claiming nearest neighbor is guaranteed O(log n)
- using int64 subtraction before widening and overflowing distance arithmetic
- forgetting to update bounding boxes
- pruning on an invalid distance lower bound
- confusing inclusive cached bounding boxes with half-open query rectangles
- claiming this teaching build is O(n log n) despite sorting every recursive subproblem
- ignoring duplicate coordinates
