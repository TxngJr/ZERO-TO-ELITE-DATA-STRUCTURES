# Course State

Current Batch: 40

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
- 097 Thread-Safe Queue / Map
- 098 Lock-Free Data Structures
- 099 Wait-Free Data Structures
- 100 Atomic Data Structures / CAS
- 101 Concurrent Hash Map
- 102 Concurrent Queue / Ring Buffer
- 103 Memory Pool / Object Pool
- 104 Arena Allocator
- 105 Slab Allocator Concepts
- 106 Free List
- 107 Garbage-Collected Data Structures
- 108 Cache-Aware Data Structures
- 109 Cache-Oblivious Data Structures
- 110 External-Memory Data Structures
- 111 Disk-Based Data Structures
- 112 Database Index Structures
- 113 File-System Data Structures
- 114 Compiler Data Structures
- 115 Operating-System Data Structures
- 116 Networking Data Structures
- 117 AI/ML Data Structures
- 118 Vector Search Structures
- 119 Game Data Structures
- 120 Merkle Tree

Current Chapter: 120

Next Chapter: 121 Merkle Patricia Trie

Concepts Covered:
- all prior Chapters 001–117
- exact flat vector indexes
- L2-squared and cosine distance
- bounded max-heaps for deterministic top-k selection
- generation-safe game entity handles
- sparse-set position components and swap-remove
- uniform spatial grids with stale-index version detection
- SHA-256 hashing
- Merkle leaf/internal domain separation
- odd-node duplication
- Merkle roots and inclusion proofs
- proof verification from sibling hashes

Structures Implemented:
- all previous structures
- VectorIndex
- GameWorld
- SpatialGrid
- MerkleTree
- MerkleProof

Tests Added:
- Vector Search: 20,000×8 vectors, 100 exact top-10 L2 queries against independent full-sort reference, cosine self-match
- Game Structures: 50,000 entities/positions, stale-handle reuse, sparse-set validation and exact 1,250-result AABB query
- Merkle Tree: SHA-256 known vector, 4,097 leaves, inclusion proofs every 37th leaf, tamper rejection and single-leaf proof

Benchmarks Added:
- vector index: 50,000×32 vectors and 200 exact top-10 queries
- game structures: 100,000 entities, grid rebuild and 10,000 AABB queries
- Merkle tree: 100,000×32-byte leaves and 10,000 proof verifications

Known Dependencies:
- 121 extends authenticated-tree ideas into Merkle Patricia Trie
- 122 covers blockchain-oriented block/transaction/state structures
- 123 covers graph-database adjacency/index structures

Open Problems:
- none if Batch 40 full Fedora ASan/UBSan CI passes and existing TSan 096–102 remains green

Cross References:
- 118 turns Chapter 117 embeddings into an exact nearest-neighbor baseline
- 119 combines generation handles, sparse sets and spatial partitioning for game-style world state
- 120 begins cryptographic/distributed structures with SHA-256 authenticated trees

Coverage: 120 / 170 chapters
