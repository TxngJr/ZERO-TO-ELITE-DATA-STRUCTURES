# Theory — Trie

## Prefix decomposition

A key x0,x1,...,x(L-1) corresponds to a path of L labeled edges from root.

Two keys share exactly their common-prefix path.

## Terminal-node invariant

A path may exist without representing a key.

The set represented by Trie is:

    all root-to-node paths whose final node has terminal=true

## Lexicographic DFS

If child labels are visited ascending and terminal current key is emitted before descendants, traversal yields byte-lexicographic order.

This follows recursively from lexicographic ordering:
- prefix comes before longer extension
- at first differing byte, smaller byte comes first.

## Sparse vs Dense

Dense:
    O(sigma) pointers per node
    O(1) child lookup

Sparse sorted list:
    O(degree) pointers/edges
    O(degree) lookup

Representation choice depends on alphabet size and branching density.
