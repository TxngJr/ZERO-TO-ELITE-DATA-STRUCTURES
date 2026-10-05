# Course State

Current Batch: 20

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

Current Chapter: 060

Next Chapter: 061 Suffix Automaton

Concepts Covered:
- all prior Chapters 001–057
- sparse uniform-grid spatial hashing
- correct floor division for negative grid coordinates
- separate-chaining cell hashing and rehashing
- cell-size trade-offs and candidate filtering
- suffix lexicographic indexing
- prefix-doubling suffix-array construction
- inverse suffix ranks
- Kasai LCP construction
- binary-search pattern ranges over suffix arrays
- compressed suffix tries
- unique terminator/sentinel outside byte alphabet
- compressed edge labels as source ranges
- pattern locus and descendant suffix leaves
- explicit distinction between naive O(n^2) suffix-tree build and Ukkonen O(n)

Structures Implemented:
- all previous structures
- IntSpatialHash
- ByteSuffixArray
- ByteSuffixTree

Tests Added:
- Spatial Hash randomized differential queries: 15,000 over 6,000 points
- negative-grid boundary tests
- Suffix Array banana/binary/randomized naive-order comparison
- Suffix Tree banana/binary/repeated/randomized occurrence comparisons

Benchmarks Added:
- Spatial Hash cell-size comparison benchmark
- Suffix Array build scaling benchmark
- naive compressed Suffix Tree build scaling benchmark

Known Dependencies:
- 061 introduces Suffix Automaton states, suffix links and substring language
- 062 introduces Aho-Corasick failure links for multi-pattern matching
- 063 studies polynomial/rolling hash substring structures

Open Problems:
- none if Batch 20 CI passes

Coverage: 60 / 170 chapters
