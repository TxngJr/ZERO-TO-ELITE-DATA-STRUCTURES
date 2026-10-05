# Course State

Current Batch: 06

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

Current Chapter: 018

Next Chapter: 019 Binary Search Tree

Concepts Covered:
- previous Chapters 001–015
- rooted-tree terminology and edge-count theorem
- depth/height/width
- full/perfect/complete/skewed/balanced shape vocabulary
- structural induction
- pointer-based Binary Tree ownership
- parent/left/right invariants
- foreign-node rejection and subtree deletion
- recursive height
- preorder/inorder/postorder
- recursive vs explicit-stack DFS
- level-order BFS with queue
- height-vs-width auxiliary-space analysis

Structures Implemented:
- all previous structures
- overflow-aware tree-math helpers
- IntBinaryTree with parent links
- recursive and iterative DFS traversals
- level-order traversal

Tests Added:
- perfect-tree math/height-bound tests
- binary-tree ownership/invariant tests
- 1023-node complete-tree structural test
- foreign-node mutation rejection
- recursive-vs-iterative traversal differential tests
- complete, irregular, skewed and empty traversal cases

Benchmarks Added:
- tree shape structural bounds
- skewed-tree height scan
- recursive vs iterative preorder benchmark

Known Dependencies:
- Chapter 019 adds BST ordering and search/insert/delete
- Chapter 020 introduces balancing concepts/rotations
- Chapter 021 implements AVL balance invariant

Open Problems:
- none if Batch 06 CI passes

Cross References:
- 005 recursion → recursive traversals
- 009 Stack → iterative DFS
- 010 Queue → level-order BFS
- 012 complete binary heap shape → tree-shape terminology
- 017 structural invariants → 019 BST ordering invariant
- 018 inorder → sorted traversal once BST invariant exists

Coverage: 18 / 170 chapters
