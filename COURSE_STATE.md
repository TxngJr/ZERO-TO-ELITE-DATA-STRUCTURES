# Course State

Current Batch: 18

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

Current Chapter: 054

Next Chapter: 055 Quadtree

Concepts Covered:
- all prior Chapters 001–051
- subtree cardinality augmentation in balanced BSTs
- O(log n) rank/select and rank-difference range counting
- metadata repair through AVL rotations/deletion
- Cartesian Tree sequence-order + heap-order dual invariant
- linear monotonic-stack Cartesian construction
- stable duplicate tie policy and original-index node identity
- direct RMQ through Cartesian topology and its O(height) limitation
- RMQ/LCA connection
- alternating-axis KD partitioning
- median-by-count static balancing
- subtree spatial bounding boxes
- rectangle pruning/full-cover subtree counting
- nearest-neighbor branch-and-bound
- point-to-box distance lower bounds
- safe widened coordinate distance arithmetic
- Range Tree vs KD-Tree time/space trade-offs

Structures Implemented:
- all previous structures
- IntOrderStatTree
- IntCartesianTree
- IntKDTree

Tests Added:
- Order Statistic Tree randomized differential workload: 40,000 operations
- Cartesian Tree randomized RMQ: 50,000 queries
- Cartesian duplicate and monotonic height-chain tests
- KD-Tree randomized rectangle/nearest queries: 12,000
- KD duplicate-coordinate and sizes 1..129 validation

Benchmarks Added:
- Order Statistic rank/select scaling
- Cartesian shape/build benchmark
- KD build and mixed nearest/range-query benchmark

Known Dependencies:
- 055 introduces hierarchical 2D quadrant subdivision
- 056 extends spatial subdivision to 3D octants
- 057 introduces bounding-rectangle hierarchy with R-Tree

Open Problems:
- none if Batch 18 CI passes

Coverage: 54 / 170 chapters
