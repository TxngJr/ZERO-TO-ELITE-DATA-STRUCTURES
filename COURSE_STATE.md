# Course State

Current Batch: 11

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

Current Chapter: 033

Next Chapter: 034 B* Tree

Concepts Covered:
- all prior Chapters 001–030
- TST low/equal/high character branching
- TST prefix lookup and lexicographic traversal
- B-Tree minimum degree / occupancy / fanout
- split-before-descent insertion
- B-Tree predecessor/successor deletion
- sibling borrow, merge and root shrink
- equal leaf-depth invariant
- page/block-oriented tree motivation
- B+ leaf-only logical records
- copied internal separator keys
- equality routing to right child
- linked leaf chain
- range scanning without repeated root descent
- separator recomputation after borrow/merge

Structures Implemented:
- all previous structures
- ByteTST
- IntBTree
- IntBPlusTree

Tests Added:
- TST basic/prefix/delete/binary/empty/lexicographic tests
- TST randomized set workload: 20,000 operations
- B-Tree sequential split/delete tests
- B-Tree randomized differential: 30,000 operations each for t=2,3,8
- B+ sequential range/delete tests
- B+ randomized set/range tests: 25,000 operations each for t=2,3,8

Benchmarks Added:
- TST vs Trie vs Radix lookup
- B-Tree fanout t=2/4/16/64
- B+ range scan vs B-Tree inorder filtering

Known Dependencies:
- 034 B* Tree increases occupancy through sibling redistribution before split
- 035 LSM Tree changes from in-place page trees to buffered sorted runs
- 036 Skip List introduces randomized layered ordered lists

Open Problems:
- none if Batch 11 CI passes

Coverage: 33 / 170 chapters
