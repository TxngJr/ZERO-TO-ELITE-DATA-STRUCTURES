# Course State

Current Batch: 08

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

Current Chapter: 024

Next Chapter: 025 Heap

Concepts Covered:
- previous Chapters 001–021
- Red-Black color and black-height invariants
- insert recoloring and rotations
- delete extra-black/sibling-case fix-up
- deterministic logarithmic Red-Black height
- self-adjusting Splay Trees
- Zig / Zig-Zig / Zig-Zag
- representation-mutating search
- worst-case vs amortized complexity
- Treap dual BST/heap invariants
- random-priority expected balance
- deterministic priority tie-breaking
- merge-based Treap deletion
- deterministic invariants vs probabilistic complexity assumptions

Structures Implemented:
- all previous structures
- IntRedBlackTree
- IntSplayTree
- IntTreap

Tests Added:
- Red-Black deterministic insert/delete sequences
- 40,000-step randomized Red-Black differential test
- Red-Black sorted-input height test
- Splay root-after-access behavior
- 30,000-step randomized Splay differential test
- Treap explicit-priority/tie-break tests
- Treap sorted-random-priority height test
- 35,000-step randomized Treap differential test

Benchmarks Added:
- Red-Black vs AVL vs plain BST
- Splay hot-key access workload
- Treap vs plain BST on sorted keys

Known Dependencies:
- Chapter 025 revisits heap as a full structure rather than only Priority Queue
- Chapter 026 generalizes to D-ary Heap
- Chapter 027 introduces meldable Binomial Heap

Open Problems:
- none if Batch 08 CI passes

Cross References:
- 020 rotations → Red-Black and Splay mechanics
- 021 AVL → deterministic balance comparison
- 004 amortized analysis → Splay guarantees
- 012 heap invariant → Treap priority order
- 132 Randomized Structures later formalizes randomized guarantees

Coverage: 24 / 170 chapters
