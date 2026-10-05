# Course State

Current Batch: 07

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

Current Chapter: 021

Next Chapter: 022 Red-Black Tree

Concepts Covered:
- previous Chapters 001–018
- strict BST ordering and duplicate policy
- search/insert/delete in O(h)
- transplant
- min/max and successor/predecessor
- inorder sortedness proof
- sorted-insertion degeneration
- left/right rotations
- LL/RR/LR/RL restructuring
- ordering invariant vs balance invariant
- balance factor
- stored height metadata
- AVL insert rebalancing
- AVL delete rebalancing
- Fibonacci-like AVL height bound

Structures Implemented:
- all previous structures
- IntBST
- RotBST teaching rotation structure
- IntAVL self-balancing BST

Tests Added:
- 30,000-step randomized BST differential test
- BST delete-case tests
- successor/predecessor tests
- sorted BST height degeneration
- rotation inorder-preservation tests
- LR/RL rotation tests
- AVL LL/RR/LR/RL tests
- AVL sorted-insert height test
- AVL deletion rebalancing tests
- 40,000-step randomized AVL differential test

Benchmarks Added:
- sorted vs shuffled plain-BST shape
- manual rotation height demonstration
- AVL vs plain BST on sorted insertions

Known Dependencies:
- Chapter 022 introduces Red-Black color/black-height invariants
- Chapter 023 Splay Tree uses access-driven rotations
- Chapter 024 Treap combines BST order with heap priorities

Open Problems:
- none if Batch 07 CI passes

Cross References:
- 018 inorder traversal → BST sorted order
- 019 O(h) operations → motivation for balance
- 020 rotations → AVL repair primitive
- 021 AVL metadata → future augmented/order-statistic trees
- 012 Heap → Treap priority invariant later

Coverage: 21 / 170 chapters
