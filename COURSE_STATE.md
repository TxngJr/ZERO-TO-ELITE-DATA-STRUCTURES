# Course State

Current Batch: 12

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

Current Chapter: 036

Next Chapter: 037 Disjoint Set / Union-Find

Concepts Covered:
- all prior Chapters 001–033
- B* high-occupancy motivation
- sibling redistribution before split
- 2-to-3 split
- B* root occupancy exceptions
- transactional rebuild deletion vs production multi-sibling delete repair
- LSM mutable MemTable / immutable run architecture
- sequence numbers and newest-write-wins visibility
- tombstones
- flush and full compaction
- read/write/space amplification
- leveled vs size-tiered compaction preview
- Skip List layered ordered links
- geometric random height distribution
- expected vs worst-case complexity
- range scan and LSM MemTable connection

Structures Implemented:
- all previous structures
- IntBStarTree
- IntLSMTree
- IntSkipList

Tests Added:
- B* order/occupancy/root split tests
- B* rebuild-deletion tests
- B* randomized differential workload: 12,000 operations
- LSM tombstone/version/compaction tests
- LSM randomized workload: 25,000 operations
- Skip List basic/range tests
- Skip List randomized differential workload: 40,000 operations

Benchmarks Added:
- B* occupancy/fanout vs B-Tree
- LSM read amplification before/after full compaction
- Skip List vs B-Tree lookup

Known Dependencies:
- 037 Union-Find introduces set partitions and near-constant amortized operations
- 038 Graph Fundamentals starts graph terminology/invariants
- 039 Graph Representations compares adjacency list/matrix/edge list

Open Problems:
- none if Batch 12 CI passes

Coverage: 36 / 170 chapters
