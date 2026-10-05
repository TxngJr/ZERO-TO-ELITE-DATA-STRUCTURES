# Course State

Current Batch: 13

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

Current Chapter: 039

Next Chapter: 040 Graph Traversal Structures

Concepts Covered:
- all prior Chapters 001–036
- partition/equivalence-class model
- disjoint-set forest
- union by size
- path compression
- inverse-Ackermann amortized complexity
- component count and component size
- graph G=(V,E)
- directed vs undirected graphs
- weights, degree, in-degree, out-degree
- walk/trail/path/cycle/connectivity terminology
- Handshaking Lemma
- sparse vs dense graph bounds
- Edge List baseline
- Adjacency Matrix
- sorted Adjacency List
- logical edges vs physical arcs/cells
- representation-independent graph semantics
- workload/density/memory trade-offs

Structures Implemented:
- all previous structures
- IntDSU
- IntGraph edge-list baseline
- GraphRepr Edge List / Matrix / Adjacency List

Tests Added:
- DSU deterministic component tests
- DSU randomized differential workload: 50,000 operations
- undirected degree-sum tests
- directed in/out-degree sum tests
- cross-representation randomized replay: 12,000 operations for directed and undirected modes
- neighbor-set equivalence checks
- representation validators after mutations

Benchmarks Added:
- DSU repeated-find/depth benchmark
- edge-list lookup scaling with E
- sparse/dense representation lookup and allocated-byte comparison

Known Dependencies:
- 040 introduces BFS/DFS traversal workspaces and frontier/visited structures
- 041 specializes structures for DAGs and topological dependencies
- 042 returns to sparse/dense representation trade-offs with broader workload analysis

Open Problems:
- none if Batch 13 CI passes

Coverage: 39 / 170 chapters
