# Implementation — IntOrderStatTree

Node:
- int64_t key
- int height
- size_t subtree_size
- left/right pointers

Tree:
- root
- total size

Refresh after any child change:
- recompute height
- recompute subtree_size

Rotations refresh lower node first, then new root.
