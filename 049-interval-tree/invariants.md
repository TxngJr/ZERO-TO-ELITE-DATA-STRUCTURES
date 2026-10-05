# Invariants — Interval Tree

1. every interval satisfies low < high
2. BST order is lexicographic by (low,high)
3. exact duplicates do not exist
4. height cache equals 1+max child height
5. AVL balance factor is in [-1,1]
6. max_high equals max endpoint over the whole subtree
7. rotations preserve in-order key order
8. deletion recomputes both height and max_high on the return path
