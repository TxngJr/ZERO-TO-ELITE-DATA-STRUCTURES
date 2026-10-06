# Course State

Current Batch: 31

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
- 049 Interval Tree
- 050 Interval Heap
- 051 Range Tree
- 052 Order Statistic Tree
- 053 Cartesian Tree
- 054 KD-Tree
- 055 Quadtree
- 056 Octree
- 057 R-Tree
- 058 Spatial Hashing
- 059 Suffix Array
- 060 Suffix Tree
- 061 Suffix Automaton
- 062 Aho-Corasick Automaton
- 063 Rolling Hash Structures
- 064 Bitset
- 065 Bitmap
- 066 Bit Vector
- 067 Bit Trie / XOR Trie
- 068 Bloom Filter
- 069 Counting Bloom Filter
- 070 Cuckoo Filter
- 071 Cuckoo Hashing
- 072 Perfect Hashing
- 073 Consistent Hashing
- 074 Probabilistic Data Structures
- 075 HyperLogLog
- 076 Count-Min Sketch
- 077 Count Sketch
- 078 Reservoir Sampling Structures
- 079 Linked Hash Map
- 080 Ordered Map / Ordered Set
- 081 Multiset / Multimap
- 082 Circular Buffer / Ring Buffer
- 083 Gap Buffer
- 084 Rope
- 085 Piece Table
- 086 Inverted Index
- 087 Posting List
- 088 Sparse Matrix Representations
- 089 Matrix/Tensor Storage Layout
- 090 Compressed Data Structures
- 091 Succinct Data Structures
- 092 Persistent Data Structures
- 093 Immutable Data Structures

Current Chapter: 093

Next Chapter: 094 Functional Data Structures

Concepts Covered:
- all prior Chapters 001–090
- succinct vs compressed/compact representations
- packed static bit vectors with rank/select
- rank directory metadata and padding correctness
- persistence taxonomy and version branching
- path copying and structural sharing
- stable-address arena lifetime and transactional allocation rollback
- immutable build-once/read-many structures
- frozen open-addressing hash sets without tombstones
- immutability vs persistence vs copy-on-write

Structures Implemented:
- all previous structures
- SuccinctBitVector
- PersistentIntSet with PSetArena
- FrozenIntSet

Tests Added:
- Succinct Bit Vector: boundary sizes around 64/512-bit packing plus randomized 200,000-bit rank/select differential checks
- Persistent Set: 1,200 randomized insert/erase operations with retained historical snapshots and repeated old-version verification
- Immutable Set: 100,000-value randomized build/dedup plus exhaustive 50,000-domain membership checks and 20,000 guaranteed misses

Benchmarks Added:
- Succinct Bit Vector: 4,000,000 bits and 200,000 rank queries with payload vs total-structure byte accounting
- Persistent Set: 20,000 retained versions with final height, arena-node and block-growth reporting
- Immutable Set: 200,000 unique keys and 1,000,000 membership queries with capacity/load/storage reporting

Known Dependencies:
- 094 introduces Functional Data Structures
- 095 studies Copy-on-Write Structures
- 096 begins Concurrent Data Structures

Open Problems:
- none if Batch 31 CI passes

Cross References:
- 091 builds on Chapters 064, 066 and 090
- 092 generalizes persistence beyond Chapter 046
- 093 contrasts immutable snapshots with persistent versioned updates

Coverage: 93 / 170 chapters
