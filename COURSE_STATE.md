# Course State

Current Batch: 33

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

Current Chapter: 099

Next Chapter: 100 Atomic Data Structures / CAS

Concepts Covered:
- all prior Chapters 001–096
- bounded MPMC queue with mutex + condition variables
- queue close/drain semantics and predicate-based condition waiting
- striped thread-safe integer map
- compound-operation atomicity boundaries
- lock-free progress via CAS retry loops
- Treiber-style stack with bounded lifetime node pool
- ABA/reclamation boundary and no-reuse teaching design
- platform atomic lock-free verification
- wait-free per-operation reasoning
- SPSC ring with release/acquire head/tail publication
- distinction between wait-free try operation and caller retry loop

Structures Implemented:
- all previous structures
- ThreadSafeQueue
- ThreadSafeIntMap
- LockFreeStack
- WaitFreeSpscRing

Tests Added:
- Thread-Safe Queue: 4 producers + 4 consumers transferring 20,000 unique values exactly once
- Thread-Safe Map: 8-thread insertion/removal with exact odd-key verification
- Lock-Free Stack: 8-thread 40,000-value push/pop plus mixed 4-producer/4-consumer stress
- Wait-Free SPSC Ring: 500,000 ordered transfers with exact FIFO verification
- TSan now covers Chapters 096–099

Benchmarks Added:
- Thread-Safe Queue: 4 producers + 4 consumers transferring 400,000 items
- Lock-Free Stack: 8 threads × 100,000 pushes with CAS-failure reporting
- Wait-Free SPSC Ring: 2,000,000 ordered transfers

Known Dependencies:
- 100 deepens atomic data structures and CAS
- 101 builds a production-oriented Concurrent Hash Map
- 102 specializes concurrent Queue / Ring Buffer
- 103 begins Memory Pool / Object Pool

Open Problems:
- none if Batch 33 full ASan/UBSan CI and concurrent TSan CI both pass

Cross References:
- 097 specializes lock-based structures from Chapter 096
- 098 relies on Chapter 096 memory-order/CAS foundations
- 099 contrasts bounded wait-free SPSC operations with Chapter 098 lock-free retry loops

Coverage: 99 / 170 chapters
