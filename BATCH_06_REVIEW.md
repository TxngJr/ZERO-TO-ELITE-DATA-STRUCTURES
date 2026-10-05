# Batch 06 — Negative Review and Audit

## Scope

- 016 Trees Fundamentals
- 017 Binary Tree
- 018 Tree Traversal

## Beginner Review

Risk: binary tree is confused with BST.
Fix: Chapter 016/017 explicitly separate structural and ordering invariants.

Risk: height is stated ambiguously.
Fix: course declares edge-height: root-only tree has height 0.

Risk: all tree operations are assumed O(log n).
Fix: complexity remains parameterized by h and skewed examples show h=Theta(n).

## Correctness Review

Tree math:
- overflow-aware perfect-tree count helpers
- empty node-count rejected for size_t height API

Binary Tree:
- opaque nodes prevent direct pointer rewiring
- root parent NULL
- child parent back-links
- foreign-node mutation rejected
- subtree detached before destruction
- size tracks removed subtree node count
- validator checks reachable count and parent consistency

Traversal:
- output capacity checked before work
- empty tree succeeds with zero output
- recursive and iterative orders cross-validated
- preorder right-before-left explicit stack rule
- inorder ancestor-stack invariant
- two-stack postorder order verified
- level-order queue visits left before right

## Memory / Lifetime Review

- tree owns nodes
- returned node handles are borrowed
- handles inside removed subtree become invalid
- iterative traversal allocations check multiplication overflow
- recursive algorithms document O(h) stack risk
- mutation during traversal is outside contract

## Complexity Review

- full traversal Theta(n)
- DFS recursive stack O(h)
- BFS logical queue O(w)
- iterative implementations may allocate n-pointer work arrays even when logical occupancy is smaller
- no unqualified O(log n) tree claim

## Testing Review

Covers:
- tree formulas
- complete-tree structure
- foreign node rejection
- subtree deletion
- complete/irregular/skewed/empty traversal
- recursive-vs-iterative differential order
- too-small output buffer

## Definition of Done

Batch 06 is complete only if Fedora CI configures, builds with ASan/UBSan and passes the complete CTest suite. CI failure overrides documentation labels.
