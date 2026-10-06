# Course State

Current Batch: 30

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

Current Chapter: 090

Next Chapter: 091 Succinct Data Structures

Concepts Covered:
- all prior Chapters 001–087
- COO/CSR/CSC sparse representations
- sparse duplicate canonicalization and zero elimination
- CSR SpMV and sparse access-pattern trade-offs
- tensor shape/stride/offset storage model
- row-major vs column-major layout
- metadata-only permutation and slicing
- contiguous vs strided views
- compressed monotonic sequences
- block checkpoints and local decoding
- delta + varint coding trade-offs
- compression payload vs metadata accounting

Structures Implemented:
- all previous structures
- CooMatrix / CsrMatrix / CscMatrix
- TensorLayout
- U64CompressedSeq

Tests Added:
- Sparse Matrix: 5,000 random COO updates canonicalized and compared against a dense 37x29 reference plus SpMV
- Tensor Layout: row/column-major offset, transpose permutation, stepped slice and invalid-view checks
- Compressed Sequence: 100k monotonic values with random-access and lower_bound differential checks

Benchmarks Added:
- Sparse Matrix 5,000x5,000 matrix with 8 generated entries per row and 100 SpMV runs
- Tensor Layout row-major vs column-oriented traversal over 2048x2048 storage
- Compressed Sequence 1M monotonic values with block_size=128 and sampled random-access reads

Known Dependencies:
- 091 introduces Succinct Data Structures
- 092 revisits Persistent Data Structures as a general design family
- 093 studies Immutable Data Structures

Open Problems:
- none if Batch 30 CI passes

Coverage: 90 / 170 chapters
