# Course State

Current Batch: 28

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

Current Chapter: 084

Next Chapter: 085 Piece Table

Concepts Covered:
- all prior Chapters 001–081
- bounded FIFO circular storage
- wrap-around index mapping
- explicit full/empty semantics
- edit locality via movable gaps
- prefix/gap/suffix physical-vs-logical text model
- geometric gap growth
- tree-based sequence representation
- subtree length/weight navigation
- split/merge editing with implicit randomized treaps
- byte-level vs Unicode-aware editing boundaries

Structures Implemented:
- all previous structures
- U64RingBuffer
- GapBuffer
- ByteRope

Tests Added:
- Ring Buffer: 50,000 randomized push/pop/get operations against an array queue model
- Gap Buffer: 20,000 randomized insert/erase/get operations against a byte-array text model
- Rope: 20,000 randomized insert/erase/get operations against a byte-array text model

Benchmarks Added:
- Ring Buffer 10M bounded FIFO updates
- Gap Buffer 500k local insertions
- Rope 100k sequential byte insertions plus indexed reads

Known Dependencies:
- 085 introduces Piece Table editor storage
- 086 introduces Inverted Index term-to-document mappings
- 087 introduces Posting List compression/intersection foundations

Open Problems:
- none if Batch 28 CI passes

Coverage: 84 / 170 chapters
