# Changelog

## Batch 20 — Chapters 058–060

Added:
- sparse 2D Spatial Hash with power-of-two separate-chaining table
- mathematical floor division for negative coordinates
- stable cell-arena indices across realloc
- transactional rollback of newly published empty cell when point-bucket allocation fails
- half-open rectangle query by touched-cell enumeration and exact boundary filtering
- 15,000 randomized spatial-hash queries over 6,000 points
- prefix-doubling byte Suffix Array
- explicit O(n log^2 n) qsort-based build complexity
- singleton-text suffix-array initialization guard
- inverse ranks and O(n) Kasai LCP construction
- binary-safe substring binary search/count/report
- randomized suffix ordering and pattern-count comparisons against naive implementations
- compressed byte Suffix Tree with symbol 256 unique sentinel
- range-based edge labels without substring copies
- explicit suffix leaves and cached descendant leaf counts
- transactional edge split publication after node/edge capacity is secured
- substring contains/count/report from pattern locus
- explicit O(n^2) naive suffix insertion complexity, with Ukkonen discussed but not misattributed
- randomized suffix-tree occurrence checks, repeated-character stress and binary-byte coverage
- Batch 20 negative review

## Batch 19 — Chapters 055–057

Added:
- dynamic bucketed 2D point-region Quadtree
- half-open quadrant partitioning with overflow-safe integer midpoint
- duplicate-coordinate handling with max-depth / unsplittable-cell fallback
- exact subtree_size and node-count validation
- transactional pre-reservation before Quadtree bucket redistribution
- 12,000 randomized Quadtree range queries over 5,000 points
- dynamic 3D Octree with 8-bit octant routing
- bucketed leaf splitting and coincident-point protection
- exact 3D box pruning and subtree full-cover counting
- transactional pre-reservation before Octree redistribution
- 8,000 randomized Octree box queries over 3,000 points
- dynamic 2D R-Tree with fixed M=4, m=2 teaching capacity
- least-area-enlargement subtree selection
- quadratic node split with minimum-fill enforcement
- propagated splits and automatic root growth
- preallocated full-parent/root split nodes to avoid half-propagated structural publication
- half-open rectangle overlap count/report
- validator for exact child MBRs, occupancy, balanced leaf depth, size and node count
- 12,000 randomized R-Tree overlap queries over 5,000 rectangles
- spatial scaling benchmarks for all three structures
- Batch 19 negative review

## Batch 18 — Chapters 052–054

Added:
- AVL Order Statistic Tree augmented with exact subtree_size metadata
- O(log n) rank, select and half-open rank-difference range counting
- 40,000-operation randomized rank/select/mutation differential test
- rank/select scaling benchmark
- static min Cartesian Tree using original sequence indices as node identities
- Theta(n) monotonic-stack construction with stable earlier-index duplicate tie policy
- direct O(height) RMQ plus explicit worst-case chain analysis
- 50,000 randomized Cartesian RMQ comparisons and monotonic-shape tests
- Cartesian build/height benchmark contrasting ascending and pseudo-random inputs
- static balanced 2D KD-Tree with alternating x/y median splits
- exact subtree bounding boxes and subtree_size summaries
- half-open rectangle count/report with bounding-box pruning
- nearest-neighbor branch-and-bound using long-double squared distances
- deterministic equal-distance tie rule by x/y/id
- 12,000 randomized KD range/nearest queries against naive scans
- duplicate-coordinate and sizes 1..129 KD validation
- KD build/query scaling benchmark
- Batch 18 negative review

## Batch 17 — Chapters 049–051

Added:
- AVL-based half-open Interval Tree keyed by (low,high)
- subtree max_high augmentation with rotation/deletion repair
- exact insert/remove/contains plus any-overlap and overlap-count queries
- 30,000-operation randomized Interval Tree differential test
- overlap-search scaling benchmark
- array-backed Interval Heap / double-ended priority queue
- embedded min-heap low endpoints and max-heap high endpoints
- singleton final-node handling and min/max deletion interval repair
- duplicate-preserving randomized DEPQ workload: 40,000 operations
- mixed double-ended priority queue benchmark
- static 2D Range Tree over (x,y,id) points
- median-balanced x primary tree with per-node y-sorted associated arrays
- linear associated-list merging per subtree
- half-open rectangle count/report APIs
- 30,000 randomized rectangle queries against naive scan
- duplicate-coordinate and non-power-of-two-size validation
- range-query scaling benchmark
- Batch 17 negative review

## Batch 16 — Chapters 046–048

Added:
- fully persistent Segment Tree with historical branching versions
- immutable path copying and structural sharing via arena node indices
- transactional unpublished-node rollback on update failure
- range sum/min and point queries on any historical version
- 6,000-step randomized branching persistence test with naive snapshots
- historical re-checks after all updates and path-copying node-growth benchmark
- Fenwick Tree / Binary Indexed Tree with 0-based public API and 1-based internal storage
- Theta(n) Fenwick linear build
- point add/set/get, prefix sum and half-open range sum
- 30,000-operation randomized Fenwick differential test
- Fenwick mixed update/query scaling benchmark
- static RMQ Sparse Table with precomputed floor-log table
- power-of-two block preprocessing and O(1) overlapping range-min queries
- 50,000 randomized Sparse Table RMQ checks plus non-power-of-two size coverage
- static query benchmark excluding preprocessing time
- Batch 16 negative review

## Batch 15 — Chapters 043–045

Added:
- static int64 Segment Tree with half-open interval contract
- simultaneous range-sum and range-min aggregates
- Theta(n) recursive build, point-set and O(log n) range queries
- 30,000-operation randomized Segment Tree differential test
- Segment Tree mixed update/query scaling benchmark
- Lazy Segment Tree with range-add tags
- const range queries using accumulated ancestor lazy carry
- exact deferred-tag structural validator
- 15,000-operation randomized lazy-propagation differential test
- lazy range-update vs naive-array benchmark
- Dynamic Segment Tree over uint64_t coordinate domains
- NULL-as-all-zero sparse subtree semantics
- overflow-safe midpoint, sparse point-add allocation and zero-path pruning
- allocation-failure rollback guard and node-count validation
- 20,000-operation randomized Dynamic Segment Tree differential test
- [0,10^18) sparse-universe benchmark
- Batch 15 negative review

## Batch 14 — Chapters 040–042

Added:
- reusable GraphTraversal workspace separated from graph storage
- iterative BFS with FIFO frontier, parent/depth/order and shortest unweighted distances
- iterative DFS with explicit stack
- path reconstruction and traversal-state validator
- randomized cross-representation BFS/DFS equivalence tests
- traversal benchmark across Edge List, Matrix and Adjacency List
- IntDAG with sorted outgoing vectors and cached indegrees
- cycle-safe dynamic edge insertion via reverse reachability check
- source/sink queries and Kahn topological sorting
- 15,000-operation randomized DAG differential test with reference reachability matrix
- topological-sort scaling benchmark
- AdaptiveGraph with density-driven Adj List <-> Matrix switching
- separate promote/demote thresholds for hysteresis
- transactional representation conversion and logical-edge replay
- deterministic threshold/switch preservation tests
- 20,000-operation randomized adaptive-graph differential tests in directed and undirected modes
- sparse/dense phase benchmark with switch count, memory and lookup timing
- Batch 14 negative review

## Batch 13 — Chapters 037–039

Added:
- Disjoint Set / Union-Find with union-by-size and path compression
- component count/size queries and structural validator
- 50,000-operation randomized DSU differential test against a naive partition model
- DSU depth/find benchmark
- Graph Fundamentals with explicit simple directed/undirected weighted graph contract
- edge-list baseline, degree/in-degree/out-degree APIs and handshaking-law tests
- edge-list lookup scaling benchmark
- unified GraphRepr API for Edge List, Adjacency Matrix and sorted Adjacency List
- undirected logical-edge vs physical-entry invariants
- memory-byte estimates and neighbor visitor API
- 12,000-operation randomized cross-representation replay for directed and undirected graphs
- sparse vs dense lookup/memory benchmark
- Batch 13 negative review

## Batch 12 — Chapters 034–036

Added:
- B* Tree with sibling redistribution before split
- 2-to-3 B* split and high-occupancy validation
- root-split occupancy exception handling
- transactional rebuild deletion with explicit O(n log n) teaching complexity
- 12,000-operation randomized B* differential test
- B* occupancy/fanout benchmark against B-Tree
- Mini in-memory LSM Tree with sorted MemTable and immutable runs
- sequence-number newest-wins visibility
- tombstones, flush, full compaction and materialized range scan
- 25,000-operation randomized LSM workload
- read-amplification benchmark before/after compaction
- integer Skip List with flexible-array forward pointers
- expected O(log n) search/insert/remove and O(log n + k) range
- 40,000-operation randomized Skip List differential test
- Skip List vs high-fanout B-Tree lookup benchmark
- Batch 12 negative review

## Batch 11 — Chapters 031–033

Added:
- byte-oriented Ternary Search Tree with low/equal/high branching
- empty/binary-key support, prefix count, deletion pruning and lexicographic traversal
- 20,000-operation randomized TST set test
- TST vs Trie vs Radix lookup benchmark
- configurable minimum-degree B-Tree
- CLRS-style split-child and insert-nonfull
- full B-Tree deletion with predecessor/successor, borrow, merge and root shrink
- B-Tree global-range / occupancy / equal-leaf-depth validator
- 30,000-operation randomized B-Tree differential tests for t=2,3,8
- B+ Tree with leaf-only logical keys and copied internal separators
- linked leaves and O(height + output) range scan
- B+ split, borrow, merge, separator recomputation and root shrink
- 25,000-operation randomized B+ Tree tests for t=2,3,8
- B+ range scan vs B-Tree full inorder/filter benchmark
- Batch 11 negative review

## Batch 10 — Chapters 028–030

Added:
- Fibonacci Heap with circular doubly linked root/child lists
- O(1) actual insert and destructive meld
- lazy consolidation on extract-min
- decrease-key with cut, mark and cascading cut
- delete-by-live-handle without sentinel-key tricks
- 25,000-operation randomized Fibonacci Heap test
- Binomial vs Fibonacci decrease-key benchmark
- sparse byte Trie with explicit-length binary keys
- exact/prefix lookup, prefix count, deletion pruning and lexicographic visitor
- empty-key and embedded-zero-byte tests
- Radix/Patricia compressed byte trie
- edge splitting, mid-edge prefix queries and unary-path recompression
- 20,000-operation randomized Radix Tree set test
- Trie/Radix prefix and lookup benchmarks
- Batch 10 negative review

## Batch 09 — Chapters 025–027

Added:
- Heap deep dive beyond Priority Queue
- Integer Min-Heap with bottom-up BUILD-HEAP
- in-place Heap Sort
- 30,000-operation randomized heap test
- bottom-up heapify vs repeated-push benchmark
- runtime D-ary Min-Heap with overflow-safe child indexing
- d=2/3/4/8/16 randomized validation
- branching-factor performance benchmark
- Binomial Heap forest representation
- root-list merge and degree consolidation
- destructive meld/union
- extract-min with child-list reversal
- decrease-key and delete by live node handle
- structural B_k validator
- 25,000-operation randomized Binomial Heap test
- Binomial meld vs incremental Binary Heap merge benchmark
- Batch 09 negative review

## Batch 08 — Chapters 022–024

Added:
- Red-Black Tree with color/black-height invariants
- insertion recolor/rotation fix-up
- deletion fix-up with NULL-as-black handling
- 40,000-operation randomized Red-Black differential test
- sorted-input height test and Red-Black vs AVL benchmark
- Splay Tree with Zig / Zig-Zig / Zig-Zag
- representation-mutating access and subtree join deletion
- 30,000-operation randomized Splay differential test
- hot-key locality benchmark
- Treap with BST order + min-heap priority
- deterministic (priority,key) tie-break
- pseudo-random default priorities plus explicit-priority API
- merge-based deletion
- 35,000-operation randomized Treap differential test
- Treap vs plain BST sorted-input benchmark
- Batch 08 negative review

## Batch 07 — Chapters 019–021

Added:
- Binary Search Tree with strict unique-key ordering
- search/insert/delete including 0/1/2-child cases
- min/max, predecessor/successor and transplant
- 30,000-operation randomized BST differential test
- explicit rotation teaching tree with left/right and LR/RL tests
- proof-oriented rotation/inorder preservation material
- full AVL Tree with stored height metadata
- LL/RR/LR/RL automatic rebalancing
- AVL deletion rebalancing
- 40,000-operation randomized AVL differential test
- plain-BST vs AVL sorted-input benchmark
- Batch 07 negative review

## Batch 06 — Chapters 016–018

Added:
- Trees Fundamentals vocabulary, height/shape mathematics and structural induction
- overflow-aware perfect-tree/min-max-height helpers
- pointer-owned IntBinaryTree with parent links and subtree deletion
- structural validator and foreign-node rejection
- recursive preorder/inorder/postorder
- iterative preorder/inorder/postorder
- level-order BFS
- recursive-vs-iterative differential tests
- shape/traversal benchmarks
- Batch 06 negative review

## Batch 05 — Chapters 013–015
Added hashing fundamentals, separate-chaining Hash Table, Hash Set and open-addressing String Hash Map.

## Batch 04 — Chapters 010–012
Added Queue, Deque and Priority Queue.

## Batch 03 — Chapters 007–009
Added Strings, Linked Lists and Stack.

## Batch 02 — Chapters 004–006
Added Complexity, Recursion/Iteration and Arrays.

## Batch 01 — Chapters 001–003
Added course foundations, memory, ADT, tests and Fedora CI.
