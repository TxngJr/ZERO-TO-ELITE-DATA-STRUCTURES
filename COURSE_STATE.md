# Course State

Current Batch: 09

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

Current Chapter: 027

Next Chapter: 028 Fibonacci Heap

Concepts Covered:
- all prior Chapters 001–024
- complete-tree heap representation
- Min-Heap implementation
- bottom-up BUILD-HEAP Theta(n)
- in-place Heap Sort
- arbitrary heap replacement
- D-ary index formulas and branching-factor trade-offs
- O(log_d n) upward paths
- O(d log_d n) downward extraction
- Binomial Tree B_k structure
- binary-counter analogy for root degrees
- root-list merge and consolidation
- destructive meld
- extract-min child reversal
- handle-based decrease-key/delete semantics
- pointer-forest vs array-heap trade-offs

Structures Implemented:
- all previous structures
- IntMinHeap
- in-place integer Heap Sort
- IntDaryHeap
- IntBinomialHeap

Tests Added:
- Heap randomized test: 30,000 operations
- Heap Sort differential check against qsort
- D-ary randomized tests: 20,000 operations each for d=2,3,4,8,16
- Binomial meld/decrease/delete deterministic tests
- duplicate-key Binomial Heap tests
- foreign-handle deletion rejection
- Binomial randomized test: 25,000 operations

Benchmarks Added:
- bottom-up BUILD-HEAP vs repeated push
- d=2/4/8/16 mixed push/pop benchmark
- Binomial meld vs Binary Heap incremental pop/push merge

Known Dependencies:
- 028 Fibonacci Heap builds on meldable-heap concepts and lazy consolidation
- 029 Trie begins prefix/string indexing structures
- 030 Radix/Patricia Trie compresses unary paths

Open Problems:
- none if Batch 09 CI passes

Cross References:
- 012 Priority Queue → heap as implementation
- 016 Complete Binary Tree → array heap shape
- 025 Heap → 026 branching generalization
- 025 array heap → 027 pointer/meld trade-off
- 027 Binomial Heap → 028 Fibonacci Heap

Coverage: 27 / 170 chapters
