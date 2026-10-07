# Course State

Current Batch: 41

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
- 121 Merkle Patricia Trie
- 122 Blockchain Data Structures
- 123 Graph Database Structures

Current Chapter: 123

Next Chapter: 124 Knowledge Graph Representation

Concepts Covered:
- all prior Chapters 001–120
- fixed-width 256-bit nibble keys
- radix-16 Patricia path compression
- Leaf / Extension / Branch authenticated trie nodes
- canonical SHA-256 MPT root independent of insertion order
- canonical transaction serialization
- transaction Merkle roots and block hash pointers
- append-only immutable block ownership
- transaction inclusion proofs
- full-chain commitment validation
- property-graph node/edge records
- external node-ID hash index
- outgoing and incoming CSR adjacency indexes
- sorted graph label index
- stale-index version detection
- indexed BFS traversal

Structures Implemented:
- all previous structures
- Mpt
- Blockchain
- BcTxProof
- GraphDb
- outgoing/incoming CSR graph indexes
- sorted label index

Tests Added:
- MPT: 10,000 256-bit keys inserted in two different orders with identical root, exact lookup and 1,000 value updates
- Blockchain: 200 blocks × 64 transactions, full-chain validation and transaction proof/tamper checks
- Graph DB: 20,000 nodes, 99,999 initial edges, exact label/neighbor queries, 999-hop traversal and stale-index rebuild

Benchmarks Added:
- MPT: 50,000 inserts + 200,000 lookups + root recomputation
- Blockchain: 1,000 blocks × 32 transactions + validation + 1,000 proofs
- Graph DB: 50,000 nodes + about 250,000 edges + index build + 10,000 query rounds

Known Dependencies:
- 124 adds semantic Knowledge Graph representation on top of graph/index concepts
- 125 begins explicit Data Structure Serialization
- 126 covers Memory Alignment & Padding representation effects

Open Problems:
- none if Batch 41 full Fedora ASan/UBSan CI passes and existing TSan 096–102 remains green

Cross References:
- 121 combines Chapters 030 and 120 into a deterministic authenticated Patricia structure
- 122 reuses Chapter 120 Merkle commitments inside hash-linked blocks
- 123 combines graph representation, hashing and rebuildable secondary indexes

Coverage: 123 / 170 chapters
