# Batch 08 — Negative Review and Audit

## Scope

- 022 Red-Black Tree
- 023 Splay Tree
- 024 Treap

## Red-Black Review

Checked:
- root is black
- NULL children are treated as black
- red nodes cannot have red children
- every node validates equal left/right black-height
- global BST bounds and parent links are checked
- insertion handles red-uncle, line and triangle cases
- deletion tracks replacement parent when replacement is NULL
- sibling/near/far child cases are mirrored
- root is normalized black after mutation

An early incomplete test payload was overwritten before commit; no placeholder/incomplete test remains in final tree.

## Splay Review

Checked:
- Zig, Zig-Zig and Zig-Zag use correct rotation order
- successful access splays found key to root
- miss splays last visited node without changing logical key set
- duplicate insertion splays existing key but returns false
- deletion detaches root and joins via max(left)
- inorder/BST ordering and parent links survive all splaying
- complexity is stated as O(n) worst single operation and O(log n) amortized, not worst-case logarithmic

## Treap Review

Checked:
- strict BST key ordering
- min-heap ordering on total pair (priority,key)
- equal priorities have deterministic tie-break
- randomization is not confused with invariant correctness
- default PRNG is explicitly non-cryptographic
- insertion rotates only while new node outranks parent
- merge deletion requires left-key-range < right-key-range
- parent links are repaired during recursive merge
- expected O(log n) is not presented as deterministic worst-case

## Testing Review

- Red-Black randomized differential: 40,000 operations
- Splay randomized differential: 30,000 operations
- Treap randomized differential: 35,000 operations
- validators run after randomized mutations
- inorder reference comparisons run periodically
- special deterministic rotation/color/priority cases included

## Complexity Review

Red-Black:
    deterministic O(log n) search/update

Splay:
    O(n) worst single operation
    O(log n) amortized

Treap:
    expected O(log n)
    O(n) worst-case

These three guarantees are intentionally contrasted.

## Definition of Done

Batch 08 is complete only if Fedora CI:
- configures
- builds all Chapter 001–024 targets with warnings + ASan/UBSan
- passes complete CTest suite

CI failure overrides all documentation completion labels.
