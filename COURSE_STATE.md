# Course State

Current Batch: 34

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

Current Chapter: 102

Next Chapter: 103 Memory Pool / Object Pool

Concepts Covered:
- all prior Chapters 001–099
- atomic read/modify/write operations
- strong/weak CAS semantics and contention retries
- tagged value+version CAS for ABA mitigation
- atomic packed bitset operations
- runtime lock-free atomic verification
- resizable concurrent hash map
- table RW-lock + per-bucket mutex protocol
- concurrent rehash publication and old-table lifetime
- bounded MPMC sequence-number ring buffer
- per-slot generation/sequence ownership
- release/acquire payload publication
- distinction between atomic/non-blocking APIs and formal lock-free guarantees

Structures Implemented:
- all previous structures
- AtomicBitset
- AtomicTaggedValue
- ConcurrentHashMap
- ConcurrentMpmcRing

Tests Added:
- Atomic Structures: 8-thread 8,192-bit set plus 8×10,000 tagged CAS increments and explicit ABA-tag test
- Concurrent Hash Map: 8-thread 40,000-key insert/update/remove workload with repeated concurrent resizing
- Concurrent Ring: 4 producers + 4 consumers transferring 100,000 unique values exactly once
- TSan now covers Chapters 096–102

Benchmarks Added:
- Tagged CAS: 8 threads × 100,000 increments with retry count
- Concurrent Hash Map: 8 threads × 50,000 inserts with resize/bucket metrics
- Concurrent Ring: 4 producers + 4 consumers transferring 1,000,000 items

Known Dependencies:
- 103 begins Memory Pool / Object Pool
- 104 formalizes Arena Allocator design
- 105 introduces Slab Allocator Concepts
- later memory-management chapters connect back to safe reclamation from concurrent structures

Open Problems:
- none if Batch 34 full ASan/UBSan CI and TSan 096–102 CI both pass

Cross References:
- 100 deepens the CAS/memory-order foundation from Chapters 096–099
- 101 extends Chapter 097 fixed striped map with synchronized resizing
- 102 generalizes Chapter 099 SPSC ring to MPMC coordination without overclaiming progress guarantees

Coverage: 102 / 170 chapters
