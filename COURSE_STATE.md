# Course State

Current Batch: 37

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

Current Chapter: 111

Next Chapter: 112 Database Index Structures

Concepts Covered:
- all prior Chapters 001–108
- cache-oblivious recursive/multiscale locality
- Morton / Z-order matrix layout without tile-size parameters
- power-of-two padded layouts and padding-overhead analysis
- external-memory / I/O block-transfer model
- RAM-resident block directory over blocked sorted data
- modeled logical block reads/writes
- point queries vs sequential range scans under an I/O model
- persistent binary file representations
- explicit little-endian serialization
- file magic/version/record-size metadata
- binary-search random seeks over on-disk sorted records
- lower_bound followed by sequential file range scans
- logical seek/read instrumentation vs physical I/O caveat

Structures Implemented:
- all previous structures
- CacheObliviousMatrix
- ExternalMemorySet
- DiskSortedIndex

Tests Added:
- Cache-Oblivious Matrix: 100,000 random updates against dense 257×193 reference and exact transpose
- External-Memory Set: 100,000 sorted keys, 20,000 point queries and exact range scan with block-I/O counters
- Disk Index: 50,000 persistent records, reopen/validate, 20,000 random point lookups, range scan and corrupt-header rejection

Benchmarks Added:
- Morton row-order vs physical Z-order traversal on 1024×1024 matrix
- External-memory set with 1,000,000 keys and 500,000 point queries
- Disk sorted index with 200,000 records and 100,000 point queries

Known Dependencies:
- 112 builds database-oriented index structures on top of storage/index concepts
- 113 moves into file-system structures
- 114 begins compiler data structures

Open Problems:
- none if Batch 37 full Fedora ASan/UBSan CI passes and existing TSan 096–102 remains green

Cross References:
- 109 contrasts directly with explicit tiling from Chapter 108
- 110 changes the cost model from CPU/cache behavior to block transfers
- 111 maps storage concepts to a persistent file format and random/sequential file access

Coverage: 111 / 170 chapters
