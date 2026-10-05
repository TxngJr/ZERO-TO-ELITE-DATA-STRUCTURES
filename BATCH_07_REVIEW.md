# Batch 07 — Negative Review and Audit

## Scope

- 019 Binary Search Tree
- 020 Balanced BST
- 021 AVL Tree

## Beginner Review

Risk: learner assumes every BST is O(log n).
Fix: all BST operation claims are parameterized by h; sorted insertion demonstrates h=n-1.

Risk: learner sees rotation as automatic balancing.
Fix: Chapter 020 explicitly separates rotation primitive from balance policy.

Risk: learner mixes edge-height with AVL stored node-height.
Fix: AVL documents NULL=0, leaf=1 internally and converts to edge-height at API boundary.

## Correctness Review

BST:
- strict unique-key policy
- global lower/upper-bound validation
- parent-link validation
- leaf / one-child / two-child deletion
- root transplant
- successor/predecessor
- inorder sortedness

Rotations:
- grandparent/root link updated
- middle subtree preserved
- parent pointers updated
- size and inorder sequence preserved
- LL/RR/LR/RL mechanics exercised

AVL:
- stored height updated child-before-parent after rotation
- balance factor convention left-right
- single and double rotations
- deletion rebalances while unwinding all ancestors
- ordering, stored heights and BF are all validated
- public API exposes no node handles, so successor-key replacement has no stable-node-identity conflict

## Complexity Review

Plain BST:
    O(h), worst O(n)

Rotation primitive:
    Theta(1)

AVL:
    h=O(log n)
    search/insert/remove O(log n)

Validator/traversal:
    Theta(n)

No logarithmic claim is made without an invariant establishing height control.

## Testing Review

- BST random differential test: 30,000 operations
- AVL random differential test: 40,000 operations
- duplicate policies checked
- all four AVL insertion patterns checked
- deletion rebalancing checked
- inorder reference comparisons
- sorted-input degeneration vs AVL bounded height
- rotations checked for exact inorder preservation

## Memory / API Review

- all tree containers own allocations
- AVL avoids public raw node handles
- rotations allocate/free no nodes
- recursive destruction depth of plain skewed BST remains a documented risk
- benchmarks are not universal timing claims

## Definition of Done

Batch 07 is complete only if Fedora CI configures, builds with ASan/UBSan and passes the entire CTest suite.
