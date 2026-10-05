# Course State

Current Batch: 16

Completed Chapters:
- 001 Programming Foundations
- 002 Memory Fundamentals
- 003 Abstract Data Type (ADT)
- 004 Complexity Analysis
- 005 Recursion & Iteration
- 006 Arrays
- 007 Strings
- 008 Linked Lists
- 009 Stack
- 010 Queue
- 011 Deque
- 012 Priority Queue
- 013 Hashing Fundamentals
- 014 Hash Table
- 015 Hash Set / Hash Map
- 016 Trees Fundamentals
- 017 Binary Tree
- 018 Tree Traversal
- 019 Binary Search Tree
- 020 Balanced BST
- 021 AVL Tree
- 022 Red-Black Tree
- 023 Splay Tree
- 024 Treap
- 025 Heap
- 026 D-ary Heap
- 027 Binomial Heap
- 028 Fibonacci Heap
- 029 Trie / Prefix Tree
- 030 Radix Tree / Patricia Trie
- 031 Ternary Search Tree
- 032 B-Tree
- 033 B+ Tree
- 034 B* Tree
- 035 LSM Tree
- 036 Skip List
- 037 Disjoint Set / Union-Find
- 038 Graphs Fundamentals
- 039 Graph Representations
- 040 Graph Traversal Structures
- 041 DAG Data Structures
- 042 Sparse vs Dense Graph Representation
- 043 Segment Tree
- 044 Lazy Propagation Segment Tree
- 045 Dynamic Segment Tree
- 046 Persistent Segment Tree
- 047 Fenwick Tree / Binary Indexed Tree
- 048 Sparse Table

Current Chapter: 048

Next Chapter: 049 Interval Tree

Concepts Covered:
- all prior Chapters 001–045
- persistence and immutable historical versions
- full persistence with branching updates from any prior version
- path copying and structural sharing
- arena indices instead of realloc-invalidated raw pointers
- O(log n) new nodes per persistent point update
- Fenwick/Binary Indexed Tree lowbit block geometry
- 0-based public vs 1-based internal indexing
- Theta(n) Fenwick linear construction
- prefix sums and range subtraction
- distinction between general associative Segment Tree aggregates and invertible prefix aggregates
- static Sparse Table preprocessing
- power-of-two interval blocks
- floor(log2) lookup tables
- idempotence and overlapping O(1) RMQ
- static-vs-dynamic range-query trade-offs

Structures Implemented:
- all previous structures
- PersistentSegmentTree
- IntFenwickTree
- IntSparseTable

Tests Added:
- persistent branching-version deterministic tests
- persistent randomized workload: 6,000 steps with naive version snapshots
- post-workload historical immutability re-checks
- Fenwick randomized differential workload: 30,000 operations
- Sparse Table randomized RMQ: 50,000 queries
- Sparse Table non-power-of-two sizes 1..129

Benchmarks Added:
- persistent path-copying nodes-per-update benchmark
- Fenwick mixed update/range-query scaling
- Sparse Table static O(1) RMQ query benchmark

Known Dependencies:
- 049 Interval Tree adds interval-overlap search metadata
- 050 Interval Heap adds double-ended priority-queue interval nodes
- 051 Range Tree introduces multidimensional orthogonal range searching

Open Problems:
- none if Batch 16 CI passes

Coverage: 48 / 170 chapters
