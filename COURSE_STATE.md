# Course State

Current Batch: 22

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

Current Chapter: 066

Next Chapter: 067 Bit Trie / XOR Trie

Concepts Covered:
- all prior Chapters 001–063
- packed machine-word bit representation
- logical bit to word/offset mapping
- final-word padding-bit invariants
- word-level boolean algebra
- popcount and count-trailing-zero operations
- dense bounded integer-universe bitmaps
- cached cardinality and range mutation
- bitmap set algebra and ordered iteration
- immutable ordered bit sequences
- rank1/rank0 semantics over half-open prefixes
- 0-based select1/select0
- per-word prefix-popcount directories
- binary-search select and padding-safe zero selection
- representation vs abstraction differences among Bitset, Bitmap and Bit Vector

Structures Implemented:
- all previous structures
- IntBitset
- IntBitmap
- IntBitVector

Tests Added:
- Bitset randomized differential workload: 50,000 operations
- Bitmap randomized differential workload: 40,000 operations
- Bit Vector randomized rank/select/access comparisons plus word-boundary sizes

Benchmarks Added:
- 10M-bit repeated Bitset AND benchmark
- 10M-universe Bitmap intersection benchmark
- 10M-bit Bit Vector build + 1M rank-query benchmark

Known Dependencies:
- 067 applies bitwise trie paths to integer XOR queries
- 068 introduces probabilistic Bloom Filter membership
- 069 extends Bloom Filter counters to support deletion semantics

Open Problems:
- none if Batch 22 CI passes

Coverage: 66 / 170 chapters
