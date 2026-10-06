# Course State

Current Batch: 35

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

Current Chapter: 105

Next Chapter: 106 Free List

Concepts Covered:
- all prior Chapters 001–102
- fixed-size memory/object pools
- free-slot stacks and slot ownership validation
- stride/alignment padding and fixed-capacity reuse
- bump/region allocation
- growable arena blocks
- power-of-two alignment up to 4096
- arena marks, bulk reset and stale-marker invalidation
- slab size classes 16/32/64/128/256
- per-slab free stacks and slot metadata
- live internal-fragmentation accounting
- empty-slab retention and trimming
- allocator overflow/failure-path invariants

Structures Implemented:
- all previous structures
- ObjectPool
- Arena
- SlabAllocator

Tests Added:
- Object Pool: 100,000 randomized allocate/free operations at capacity 1,024
- Arena: 50,000 allocations with sizes 1..97 and alignments 1..4096 plus mark/reset checks
- Slab: 80,000 randomized allocations/frees across five size classes and 4,096 active slots
- double-free/foreign-pointer/stale-mark checks
- ASan/UBSan + LeakSanitizer local verification

Benchmarks Added:
- Object Pool: approximately 2,000,000 allocation/release operations
- Arena: 1,000,000 allocations plus bulk reset
- Slab: approximately 1,000,000 allocation/release operations across five classes

Known Dependencies:
- 106 isolates Free List design as its own chapter
- 107 introduces Garbage-Collected Data Structures
- 108 begins Cache-Aware Data Structures

Open Problems:
- none if Batch 35 full Fedora ASan/UBSan CI passes

Cross References:
- 103 specializes reusable fixed-size allocation
- 104 changes reclamation granularity from per-object to per-region
- 105 generalizes fixed pools into multiple size classes/slabs

Coverage: 105 / 170 chapters
