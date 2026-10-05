# Pitfalls — Treap

- calling expected O(log n) a worst-case guarantee
- using ambiguous equal-priority behavior
- breaking BST order during rotation
- merging subtrees whose key ranges overlap
- forgetting parent links during recursive merge
- treating PRNG as cryptographic
- using priority as logical key identity
- assuming deterministic seed gives security randomness
- validating only BST order and ignoring heap order
