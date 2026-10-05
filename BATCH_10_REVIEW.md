# Batch 10 — Negative Review and Audit

## Scope

- 028 Fibonacci Heap
- 029 Trie / Prefix Tree
- 030 Radix Tree / Patricia Trie

## Fibonacci Heap Review

Checked:
- circular doubly linked root/child lists
- lazy insert/meld without eager consolidation
- destructive meld empties source
- roots are unmarked
- cut decrements parent degree
- cascading cut marks first child loss and cuts on later loss
- extract-min promotes children and consolidates degrees
- validator checks links, degrees, heap order, marks, size and min
- delete does not depend on INT_MIN sentinel

Handle contract:
decrease-key/delete require a valid live handle belonging to the heap.
No O(number-of-roots) membership scan is performed because a lazy root list can be O(n), which would destroy the classical O(1) amortized decrease-key bound.

## Trie Review

Checked:
- explicit byte pointer + length supports embedded zero bytes
- empty key uses root terminal
- exact key separated from prefix path
- sparse child edges strictly sorted
- duplicate inserts do not increase size
- deletion prunes only unshared non-terminal suffixes
- lexicographic DFS emits prefix before extensions
- UTF-8 bytes are not described as locale-aware characters

## Radix Review

Checked:
- edge labels non-empty
- sibling first bytes unique/sorted
- insert handles missing edge, full match, partial split and key-ending-at-split
- old suffix preserved
- prefix may terminate mid-edge
- exact key may not terminate mid-edge
- delete prunes and opportunistically recompresses unary paths
- failed optional recompression allocation leaves semantically valid structure
- visitor appends complete compressed labels

## Testing Review

- Fibonacci random insert/extract: 25,000 operations
- repeated Fibonacci decrease-key after consolidation
- Trie empty/binary/exact/prefix/delete/lexicographic tests
- Trie 1,000 generated keys
- Radix random set workload: 20,000 operations
- validators run throughout mutations

## Complexity Review

Fibonacci:
- insert/meld O(1) amortized
- decrease-key O(1) amortized under valid-handle precondition
- extract-min O(log n) amortized

Trie:
- fixed byte alphabet path operations O(L) with sparse-degree constant factor

Radix:
- compressed path comparisons O(L)
- prefix/output operations add matched subtree/output cost

## Definition of Done

Batch 10 is complete only when Fedora CI configures, builds Chapters 001–030 with warnings + ASan/UBSan and passes complete CTest.
