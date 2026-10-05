# Visual Model — Path Copying

Version 0:

          A
        /   \
       B     C
      / \   / \
     D   E  F   G

Update a leaf under C/F.

Version 1:

          A'
        /    \
       B      C'
             / \
            F'  G

Shared:
    B, D, E, G

Copied:
    A', C', F'

Old root A still points to old C/F.
