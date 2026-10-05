# Theory — Balanced BST

## Rotation correctness

Left rotation partitions keys into ordered intervals:

    keys(A) < x < keys(B) < y < keys(C)

Rotation only changes edges among x, y and B.
The inorder concatenation remains:

    A, x, B, y, C

Therefore BST ordering is preserved.

## Primitive vs Policy

Rotation is a primitive.
AVL/Red-Black are policies plus metadata/invariants.

Testing rotation separately reduces the number of things that can be wrong when implementing a self-balancing tree.

## Height Bounds

Search/update paths are O(h).

A balancing scheme becomes useful when its invariant allows proof:

    h = O(log n)

Different schemes prove this differently.
