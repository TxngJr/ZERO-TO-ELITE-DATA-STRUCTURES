# Course State

Current Batch: 26

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

Current Chapter: 078

Next Chapter: 079 Linked Hash Map

Concepts Covered:
- all prior Chapters 001–075
- one-sided nonnegative frequency estimation
- Count-Min width/depth error trade-offs
- signed turnstile frequency estimation
- random-sign collision cancellation
- median row estimator
- exact-vs-approximate frequency model comparison
- bounded-memory uniform streaming samples
- Algorithm R inclusion probability
- RNG reproducibility vs randomness quality
- rejection-based bounded random integers and modulo bias

Structures Implemented:
- all previous structures
- U64CountMinSketch
- U64CountSketch
- U64Reservoir

Tests Added:
- Count-Min: 50,000 weighted updates against exact frequencies; no-underestimate assertion
- Count Sketch: 50,000 signed updates against exact signed frequencies
- Reservoir: state/reproducibility tests and 30,000-trial k=1 uniformity experiment

Benchmarks Added:
- Count-Min 1M updates at width=4096/depth=5
- Count Sketch 1M signed updates at width=4096/depth=5
- Reservoir 10M stream updates with k=1024

Known Dependencies:
- 079 combines hash lookup with linked iteration order
- 080 studies ordered maps/sets with sorted-key semantics
- 081 introduces multiplicity and multiple-values-per-key abstractions

Open Problems:
- none if Batch 26 CI passes

Coverage: 78 / 170 chapters
