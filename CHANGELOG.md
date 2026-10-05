# Changelog

## Batch 12 — Chapters 034–036

Added:
- B* Tree with sibling redistribution before split
- 2-to-3 B* split and high-occupancy validation
- root-split occupancy exception handling
- transactional rebuild deletion with explicit O(n log n) teaching complexity
- 12,000-operation randomized B* differential test
- B* occupancy/fanout benchmark against B-Tree
- Mini in-memory LSM Tree with sorted MemTable and immutable runs
- sequence-number newest-wins visibility
- tombstones, flush, full compaction and materialized range scan
- 25,000-operation randomized LSM workload
- read-amplification benchmark before/after compaction
- integer Skip List with flexible-array forward pointers
- expected O(log n) search/insert/remove and O(log n + k) range
- 40,000-operation randomized Skip List differential test
- Skip List vs high-fanout B-Tree lookup benchmark
- Batch 12 negative review

## Batch 11 — Chapters 031–033

Added:
- byte-oriented Ternary Search Tree with low/equal/high branching
- empty/binary-key support, prefix count, deletion pruning and lexicographic traversal
- 20,000-operation randomized TST set test
- TST vs Trie vs Radix lookup benchmark
- configurable minimum-degree B-Tree
- CLRS-style split-child and insert-nonfull
- full B-Tree deletion with predecessor/successor, borrow, merge and root shrink
- B-Tree global-range / occupancy / equal-leaf-depth validator
- 30,000-operation randomized B-Tree differential tests for t=2,3,8
- B+ Tree with leaf-only logical keys and copied internal separators
- linked leaves and O(height + output) range scan
- B+ split, borrow, merge, separator recomputation and root shrink
- 25,000-operation randomized B+ Tree tests for t=2,3,8
- B+ range scan vs B-Tree full inorder/filter benchmark
- Batch 11 negative review

## Batch 10 — Chapters 028–030

Added:
- Fibonacci Heap with circular doubly linked root/child lists
- O(1) actual insert and destructive meld
- lazy consolidation on extract-min
- decrease-key with cut, mark and cascading cut
- delete-by-live-handle without sentinel-key tricks
- 25,000-operation randomized Fibonacci Heap test
- Binomial vs Fibonacci decrease-key benchmark
- sparse byte Trie with explicit-length binary keys
- exact/prefix lookup, prefix count, deletion pruning and lexicographic visitor
- empty-key and embedded-zero-byte tests
- Radix/Patricia compressed byte trie
- edge splitting, mid-edge prefix queries and unary-path recompression
- 20,000-operation randomized Radix Tree set test
- Trie/Radix prefix and lookup benchmarks
- Batch 10 negative review

## Batch 09 — Chapters 025–027

Added:
- Heap deep dive beyond Priority Queue
- Integer Min-Heap with bottom-up BUILD-HEAP
- in-place Heap Sort
- 30,000-operation randomized heap test
- bottom-up heapify vs repeated-push benchmark
- runtime D-ary Min-Heap with overflow-safe child indexing
- d=2/3/4/8/16 randomized validation
- branching-factor performance benchmark
- Binomial Heap forest representation
- root-list merge and degree consolidation
- destructive meld/union
- extract-min with child-list reversal
- decrease-key and delete by live node handle
- structural B_k validator
- 25,000-operation randomized Binomial Heap test
- Binomial meld vs incremental Binary Heap merge benchmark
- Batch 09 negative review

## Batch 08 — Chapters 022–024

Added:
- Red-Black Tree with color/black-height invariants
- insertion recolor/rotation fix-up
- deletion fix-up with NULL-as-black handling
- 40,000-operation randomized Red-Black differential test
- sorted-input height test and Red-Black vs AVL benchmark
- Splay Tree with Zig / Zig-Zig / Zig-Zag
- representation-mutating access and subtree join deletion
- 30,000-operation randomized Splay differential test
- hot-key locality benchmark
- Treap with BST order + min-heap priority
- deterministic (priority,key) tie-break
- pseudo-random default priorities plus explicit-priority API
- merge-based deletion
- 35,000-operation randomized Treap differential test
- Treap vs plain BST sorted-input benchmark
- Batch 08 negative review

## Batch 07 — Chapters 019–021

Added:
- Binary Search Tree with strict unique-key ordering
- search/insert/delete including 0/1/2-child cases
- min/max, predecessor/successor and transplant
- 30,000-operation randomized BST differential test
- explicit rotation teaching tree with left/right and LR/RL tests
- proof-oriented rotation/inorder preservation material
- full AVL Tree with stored height metadata
- LL/RR/LR/RL automatic rebalancing
- AVL deletion rebalancing
- 40,000-operation randomized AVL differential test
- plain-BST vs AVL sorted-input benchmark
- Batch 07 negative review

## Batch 06 — Chapters 016–018

Added:
- Trees Fundamentals vocabulary, height/shape mathematics and structural induction
- overflow-aware perfect-tree/min-max-height helpers
- pointer-owned IntBinaryTree with parent links and subtree deletion
- structural validator and foreign-node rejection
- recursive preorder/inorder/postorder
- iterative preorder/inorder/postorder
- level-order BFS
- recursive-vs-iterative differential tests
- shape/traversal benchmarks
- Batch 06 negative review

## Batch 05 — Chapters 013–015
Added hashing fundamentals, separate-chaining Hash Table, Hash Set and open-addressing String Hash Map.

## Batch 04 — Chapters 010–012
Added Queue, Deque and Priority Queue.

## Batch 03 — Chapters 007–009
Added Strings, Linked Lists and Stack.

## Batch 02 — Chapters 004–006
Added Complexity, Recursion/Iteration and Arrays.

## Batch 01 — Chapters 001–003
Added course foundations, memory, ADT, tests and Fedora CI.
