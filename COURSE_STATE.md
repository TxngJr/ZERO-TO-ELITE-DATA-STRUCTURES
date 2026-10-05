# Course State

Current Batch: 25

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

Current Chapter: 075

Next Chapter: 076 Count-Min Sketch

Concepts Covered:
- all prior Chapters 001–072
- circular token spaces and clockwise ownership
- virtual-node consistent hashing
- limited remapping under node membership changes
- probabilistic error contracts
- one-sided vs two-sided error categories
- memory/accuracy trade-offs
- KMV cardinality order-statistic estimation
- duplicate-insensitive sketch updates
- mergeable sketch state
- HyperLogLog registers and leading-zero ranks
- precision/register trade-offs
- HLL raw estimator and small-range correction
- register-wise-max distributed merge

Structures Implemented:
- all previous structures
- ConsistentHashRing
- U64KmvSketch
- U64HyperLogLog

Tests Added:
- Consistent Hashing 50,000-key distribution/remapping/restoration checks
- KMV exact duplicate handling and 50,000-cardinality accuracy/merge checks
- HLL empty/small/100,000-cardinality accuracy and exact merge-state equivalence

Benchmarks Added:
- Consistent Hashing 100 nodes / 12,800 ring points / 1M lookups
- KMV 1M updates at k=2048
- HLL 1M updates at p=14

Known Dependencies:
- 076 implements Count-Min Sketch for one-sided frequency estimates
- 077 implements Count Sketch for signed/unbiased-style frequency estimation
- 078 introduces Reservoir Sampling structures for bounded-memory streaming samples

Open Problems:
- none if Batch 25 CI passes

Coverage: 75 / 170 chapters
