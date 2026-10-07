# Course State

Current Batch: 39

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

Current Chapter: 117

Next Chapter: 118 Vector Search Structures

Concepts Covered:
- all prior Chapters 001–114
- fixed-capacity process tables and free-slot stacks
- generation-safe PID handles
- single-CPU process state transitions
- strict-priority FIFO run queues
- IPv4 binary prefix tries and longest-prefix match
- default routes and overlapping prefixes
- five-tuple flow tables with open addressing
- tombstones, linear probing and flow-table rehashing
- contiguous dense ML datasets
- shuffled index permutations and mini-batch plans
- embedding tables and gather operations
- overflow-safe tensor/dataset allocation sizing

Structures Implemented:
- all previous structures
- OsScheduler
- RouteTrie
- FlowTable
- MlDataset
- BatchPlan
- EmbeddingTable

Tests Added:
- OS Structures: 10,000 processes across four priorities, 1,000 dispatch/yield/block transitions, wake and stale-PID reuse checks
- Networking: 4,096 generated /24 routes plus 50,000 flow entries with updates/removals
- AI/ML: 50,000×16 dataset one-epoch permutation verification and 20,000×32 embedding table with 10,000-row gather

Benchmarks Added:
- scheduler: 40,000 ready processes and 500,000 dispatch/yield cycles
- networking: 200,000 route lookups + 100,000 flow insert/lookups
- AI/ML: 200,000×32 dataset gather epoch + 200,000 embedding gathers

Known Dependencies:
- 118 introduces Vector Search Structures over embedding/vector data
- 119 covers game-oriented entity/spatial/state structures
- 120 begins cryptographic/distributed structures with Merkle Tree

Open Problems:
- none if Batch 39 full Fedora ASan/UBSan CI passes and existing TSan 096–102 remains green

Cross References:
- 115 combines queue/free-slot/handle concepts in an operating-system scheduling model
- 116 combines trie longest-prefix routing with exact-key hash flow state
- 117 builds on tensor layout/cache-aware storage while intentionally deferring nearest-neighbor indexing to 118

Coverage: 117 / 170 chapters
