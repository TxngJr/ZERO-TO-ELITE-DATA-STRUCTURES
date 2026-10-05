# Theory — Radix / Patricia Trie

Path compression replaces unary non-terminal chains with one multi-symbol edge without changing represented keys.

For insert split:
let P be longest common prefix of remaining key K and existing edge E.

If P shorter than E:
- P becomes parent edge
- old suffix E[P:] preserves all old keys
- new suffix K[P:] becomes new branch, or middle becomes terminal if K ends at P

Compressed representation changes node boundaries, not prefix semantics.
