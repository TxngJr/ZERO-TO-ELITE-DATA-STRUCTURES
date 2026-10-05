# Batch 18 — Negative Review and Audit

## Scope

- 052 Order Statistic Tree
- 053 Cartesian Tree
- 054 KD-Tree

## Chapter 052 Review

Checked:
- implementation is an AVL set augmented with subtree_size
- subtree_size includes the node itself
- refresh recomputes both height and subtree_size
- rotations refresh lower node before new root
- duplicate insertion is rejected
- rank is defined for missing keys as number of keys strictly less than key
- select is explicitly 0-based
- count [low,high) uses rank(high)-rank(low)
- two-child deletion decrements tree.size exactly once
- validator recomputes strict BST order, height, AVL balance and subtree cardinality

Complexity:
- insert/remove/contains O(log n)
- rank/select/count-range O(log n)
- storage Theta(n)

## Chapter 053 Review

Checked:
- node identity equals original sequence index
- min Cartesian Tree preserves inorder index order
- parent value <= child value
- duplicate rule pops only strictly larger values, so equal earlier indices remain stable
- each index is pushed once and popped at most once
- construction is Theta(n)
- direct RMQ descends by index until a node lies inside query
- a node inside the query is <= every descendant by heap order
- validator uses iterative reachability and inorder checks, avoiding recursion on worst-case chains
- ascending and descending test inputs explicitly verify height n-1
- documentation does not claim balanced height

Complexity:
- build Theta(n)
- direct RMQ O(h), worst O(n)
- storage Theta(n)

## Chapter 054 Review

Checked:
- build alternates x/y split axis by depth
- median-by-count recursion gives logarithmic static height
- teaching builder sorts every recursive subproblem and is documented as O(n log^2 n)
- bounding boxes are exact subtree coordinate extrema
- subtree_size supports O(1) fully-covered rectangle count
- rectangle API uses half-open bounds while cached boxes use inclusive extrema
- nearest search widens coordinates to long double before subtraction
- bounding-box squared distance is a valid lower bound
- nearest branch pruning uses <= current best so equal-distance tie candidates remain visible
- equal-distance result tie is deterministic by x/y/id
- nearest worst case is correctly documented as O(n), not guaranteed O(log n)
- validator checks axis depth, partition bounds, bounding boxes, subtree size and unique reachability

Complexity:
- build O(n log^2 n) for this implementation
- balanced static height O(log n)
- classical 2D orthogonal range O(sqrt(n)+k)
- nearest branch-and-bound worst O(n)
- storage Theta(n)

## Testing Review

- Order Statistic Tree randomized differential: 40,000 operations
- Cartesian Tree randomized RMQ: 50,000 queries
- Cartesian monotonic inputs verify worst-case height
- KD-Tree randomized range/nearest: 12,000 queries against naive scans
- KD validation covers duplicate coordinates and sizes 1..129
- CI retains 60-second per-test watchdog

## Definition of Done

Batch 18 is complete only when Fedora CI configures/builds Chapters 001–054 with warnings + ASan/UBSan and the full CTest suite passes.
