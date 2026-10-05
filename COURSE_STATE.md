# Course State

Current Batch: 14

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

Current Chapter: 042

Next Chapter: 043 Segment Tree

Concepts Covered:
- all prior Chapters 001–039
- traversal state separated from graph storage
- visited/frontier/parent/depth/order workspaces
- BFS FIFO frontier and shortest unweighted distances
- iterative DFS explicit stack
- path reconstruction through traversal parents
- representation-sensitive traversal complexity
- DAG acyclic invariant
- sorted outgoing dependency vectors
- cached indegree metadata
- cycle-safe dynamic edge insertion
- sources, sinks and Kahn topological order
- density as E/max_possible_edges
- sparse vs dense workload trade-offs
- adaptive Adj List / Matrix backends
- hysteresis with separate promote/demote thresholds
- transactional semantic-preserving representation conversion
- conversion latency and peak-memory trade-offs

Structures Implemented:
- all previous structures
- GraphTraversal
- IntDAG
- AdaptiveGraph

Tests Added:
- deterministic BFS shortest-depth/path tests
- deterministic DFS reachability tests
- randomized cross-representation traversal comparison over 60 sources
- DAG deterministic dependency/cycle-rejection tests
- DAG randomized differential workload: 15,000 operations
- AdaptiveGraph threshold switch + edge-weight preservation tests
- AdaptiveGraph randomized differential: 20,000 operations each for directed and undirected modes

Benchmarks Added:
- BFS across Edge List / Matrix / Adj List on sparse and dense-ish graphs
- Kahn topological sort scaling
- adaptive sparse -> dense -> sparse backend transition benchmark

Known Dependencies:
- 043 Segment Tree begins range-query data structures
- 044 adds lazy propagation for range updates
- 045 moves to dynamically allocated sparse segment trees

Open Problems:
- none if Batch 14 CI passes

Coverage: 42 / 170 chapters
