# Implementation — IntBTree

Tree:
- minimum degree t
- root
- size

Node:
- leaf flag
- key_count
- keys array size 2t-1
- children array size 2t

Core:
- split_child
- insert_nonfull
- remove_from_leaf
- remove_from_internal
- borrow_from_prev
- borrow_from_next
- merge_children
- fill_child_before_descent
- validate ranges/occupancy/leaf depth
