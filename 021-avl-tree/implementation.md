# Implementation — IntAVL

Node:
- key
- height (node-height, NULL=0)

No public node pointers.

Core helpers:
- height_of
- update_height
- balance_factor
- rotate_left/right
- rebalance
- insert_rec
- remove_rec
- validate_rec

Insert/remove return tree root from recursion because rotations may replace subtree roots.

## Allocation Failure

Insertion allocates only at missing leaf.

If allocation fails:
- status propagates upward
- existing structure remains logically unchanged
- rebalance/update on unchanged path is harmless and deterministic

## Delete

0/1 child:
- return surviving child
- free removed node

2 children:
- copy successor key
- delete successor recursively

Then rebalance current subtree before return.
