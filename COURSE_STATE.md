# Course State

Current Batch: 10

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

Current Chapter: 030

Next Chapter: 031 Ternary Search Tree

Concepts Covered:
- all prior Chapters 001–027
- Fibonacci Heap lazy consolidation
- circular doubly linked root/child lists
- min pointer
- marks, cut and cascading cut
- actual vs amortized heap costs
- live-handle preconditions
- byte-oriented Trie keys with explicit lengths
- terminal nodes vs prefix paths
- sparse sorted child edges
- prefix counting and lexicographic traversal
- binary keys including embedded zero bytes
- Radix path compression
- edge splitting on partial match
- prefixes ending inside compressed edges
- deletion recompression and OOM-safe semantic fallback

Structures Implemented:
- all previous structures
- IntFibonacciHeap
- ByteTrie
- ByteRadixTree

Tests Added:
- Fibonacci deterministic meld/decrease/delete tests
- Fibonacci randomized insert/extract: 25,000 operations
- repeated decrease-key/cut validation
- Trie exact/prefix/delete/empty/binary-key tests
- Trie generated 1,000-key workload
- Radix split/mid-edge prefix/delete tests
- Radix binary/empty and lexicographic tests
- Radix randomized set workload: 20,000 operations

Benchmarks Added:
- Fibonacci vs Binomial decrease-key
- Trie prefix query vs linear scan
- Radix vs Trie lookup on long shared paths

Known Dependencies:
- 031 Ternary Search Tree combines character-wise search with BST-style branching
- 032 B-Tree begins external-memory/page-oriented balanced trees
- 033 B+ Tree moves records to leaves and supports leaf-level range scans

Open Problems:
- none if Batch 10 CI passes

Coverage: 30 / 170 chapters
