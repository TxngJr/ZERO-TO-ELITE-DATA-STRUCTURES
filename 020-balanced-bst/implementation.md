# Implementation — RotBST

Fields:
- key
- parent
- left
- right

Tree:
- root
- size

Public operations:
- insert
- find
- rotate_left(key)
- rotate_right(key)
- inorder
- height
- validate

rotate_left fails if pivot has no right child.
rotate_right fails if pivot has no left child.

No automatic balancing is performed.
