# Course State

Current Batch: 15

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

Current Chapter: 045

Next Chapter: 046 Persistent Segment Tree

Concepts Covered:
- all prior Chapters 001–042
- interval hierarchy and half-open range contract
- canonical interval decomposition
- associative aggregate / monoid viewpoint
- simultaneous sum and minimum caching
- Theta(n) Segment Tree construction
- O(log n) point updates and range queries
- lazy range-add tags
- push, pull and deferred-update invariants
- const query via accumulated lazy carry
- sparse coordinate universes
- dynamic pointer-based interval materialization
- NULL subtree as all-zero interval
- overflow-safe uint64 midpoint
- path sharing, zero-path pruning and sparse node counts
- allocation-failure rollback for dynamic update paths

Structures Implemented:
- all previous structures
- IntSegmentTree
- LazySegmentTree
- DynamicSegmentTree

Tests Added:
- Segment Tree deterministic sum/min/update tests
- Segment Tree randomized differential workload: 30,000 operations
- Lazy Segment Tree overlapping range-add/query tests
- Lazy randomized differential workload: 15,000 operations
- Dynamic Segment Tree huge-domain/pruning tests
- Dynamic randomized differential workload: 20,000 operations

Benchmarks Added:
- mixed Segment Tree point-update/range-query scaling
- Lazy Segment Tree range updates vs naive array
- Dynamic Segment Tree on sparse [0,10^18) coordinate universe

Known Dependencies:
- 046 introduces structural sharing and immutable historical Segment Tree versions
- 047 introduces Fenwick Tree for compact prefix-aggregate workloads
- 048 introduces Sparse Table for static idempotent range queries

Open Problems:
- none if Batch 15 CI passes

Coverage: 45 / 170 chapters
