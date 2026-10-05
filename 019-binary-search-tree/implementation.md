# Implementation — IntBST

Node:
- key
- parent
- left
- right

Tree:
- root
- size

Public node handles are borrowed.

Core helpers:
- find_node
- minimum_node
- maximum_node
- transplant
- free_subtree
- validate_bounds

Deletion never exposes partially invalid state after returning.

No balancing is performed here.
