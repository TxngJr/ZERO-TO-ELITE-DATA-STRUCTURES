# Course State

Current Batch: 32

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
- 094 Functional Data Structures
- 095 Copy-on-Write Structures
- 096 Concurrent Data Structures

Current Chapter: 096

Next Chapter: 097 Thread-Safe Queue / Map

Concepts Covered:
- all prior Chapters 001–093
- pure transformations and referential transparency
- two-list functional queue representation
- structural sharing and linear-history amortized reasoning
- copy-on-write sharing, refcounts and detach-on-write
- first-write latency and strong allocation-failure behavior
- thread/shared-state mental model
- race condition vs C data race
- mutex-protected representation invariants
- atomicity, visibility and memory ordering
- release/acquire publication
- linearizability, CAS, ABA, lock-free and wait-free foundations

Structures Implemented:
- all previous structures
- FunctionalQueue with immutable front/rear lists and arena-backed nodes
- CowIntVector with shared ref-counted backing storage
- ConcurrentIntSet with mutex-protected sorted dynamic storage

Tests Added:
- Functional Queue: 20,000 randomized operations, invariant checks and retained historical snapshots
- Copy-on-Write Vector: 256 clones, detach isolation, growth/pop checks and LeakSanitizer validation
- Concurrent Set: 8-thread disjoint inserts, concurrent even removals, duplicate insertion stress, exact membership and snapshot checks
- Atomic publication: release/acquire producer-consumer smoke test

Benchmarks Added:
- Functional Queue: 200,000 enqueue + dequeue operations
- Copy-on-Write Vector: 1,000,000-element payload, 2,000 clones and 100 detach writes
- Concurrent Set: 8 threads × 10,000 inserts with throughput reporting

Known Dependencies:
- 097 specializes thread-safe Queue / Map
- 098 introduces Lock-Free Data Structures
- 099 introduces Wait-Free Data Structures
- 100 deepens Atomic Data Structures / CAS

Open Problems:
- none if Batch 32 ASan/UBSan full-suite CI and Chapter 096 TSan CI both pass

Cross References:
- 094 builds on Chapters 010, 092 and 093
- 095 connects mutable arrays with sharing from Chapters 092–093
- 096 follows the required concurrency foundation before Chapters 097–102

Coverage: 96 / 170 chapters
