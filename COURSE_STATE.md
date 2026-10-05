# Course State

Current Batch: 17

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

Current Chapter: 051

Next Chapter: 052 Order Statistic Tree

Concepts Covered:
- all prior Chapters 001–048
- half-open interval overlap semantics
- AVL interval ordering by (low,high)
- subtree max_high augmentation and pruning
- exact interval mutation plus any/all overlap search
- double-ended priority queues
- interval-heap containment invariant
- embedded min heap on low endpoints
- embedded max heap on high endpoints
- singleton final interval node and two-sided deletion repair
- 2D orthogonal range searching
- median-balanced x primary search tree
- y-sorted associated arrays at every Range Tree node
- canonical x-subtrees and y binary search
- O(log^2 n) static rectangle counting
- O(log^2 n + k) reporting and Theta(n log n) storage trade-off

Structures Implemented:
- all previous structures
- IntIntervalTree
- IntIntervalHeap
- IntRangeTree

Tests Added:
- Interval Tree randomized differential workload: 30,000 operations
- Interval Heap randomized multiset/DEPQ workload: 40,000 operations
- Range Tree randomized rectangle queries: 30,000
- Range Tree duplicate-coordinate and sizes 1..129 validation

Benchmarks Added:
- Interval Tree any-overlap lookup scaling
- Interval Heap mixed insert/pop-min/pop-max benchmark
- Range Tree static rectangle-count scaling

Known Dependencies:
- 052 augments a balanced BST with subtree sizes for rank/select
- 053 studies Cartesian Tree ordering by sequence position and heap priority
- 054 begins spatial partitioning with KD-Tree

Open Problems:
- none if Batch 17 CI passes

Coverage: 51 / 170 chapters
