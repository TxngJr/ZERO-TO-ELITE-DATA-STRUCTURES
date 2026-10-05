# Course State

Current Batch: 21

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

Current Chapter: 063

Next Chapter: 064 Bitset

Concepts Covered:
- all prior Chapters 001–060
- substring language represented by suffix automaton states
- end-position equivalence and suffix links
- clone-state online SAM construction
- occurrence propagation and distinct-substring formula
- multi-pattern trie matching
- Aho-Corasick failure/output links
- duplicate pattern IDs and binary-safe match reporting
- polynomial prefix hashing
- double hash pairs and collision risk
- O(1) raw substring-hash extraction
- verified Rabin-Karp matching
- exact substring equality with hash rejection + byte verification
- hash-assisted LCP

Structures Implemented:
- all previous structures
- ByteSuffixAutomaton
- ByteAhoCorasick
- ByteRollingHash

Tests Added:
- SAM randomized pattern-count comparisons and naive distinct/repetition checks
- Aho-Corasick randomized multi-pattern matching against naive scans
- Rolling Hash randomized equality/LCP/pattern-search comparisons

Benchmarks Added:
- Suffix Automaton build/state-count scaling
- Aho-Corasick dictionary build + million-byte scan
- Rolling Hash million-byte build + 500,000 range-hash probes

Known Dependencies:
- 064 begins compact bit containers with Bitset
- 065 distinguishes bitmap use cases/representations
- 066 develops rank/select-capable Bit Vector foundations

Open Problems:
- none if Batch 21 CI passes

Coverage: 63 / 170 chapters
