# Implementation — IntTreap

Node:
- key
- uint32 priority
- parent
- left
- right

Tree:
- root
- size
- PRNG state

Heap comparator:

    lower priority wins
    if equal priority, lower key wins

Core:
- rotate_left/right
- insert_with_priority
- default insert using PRNG
- merge_nodes
- remove_rec
- validate_node

## Delete

remove_rec returns replacement subtree root.

At target:
- detach left/right conceptually
- merge both
- free target
- returned merged root receives original parent

No extra rebalancing metadata is needed.
