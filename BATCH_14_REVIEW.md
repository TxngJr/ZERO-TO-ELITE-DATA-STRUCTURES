# Batch 14 — Negative Review and Audit

## Scope

- 040 Graph Traversal Structures
- 041 DAG Data Structures
- 042 Sparse vs Dense Graph Representation

## Chapter 040 Review

Checked:
- traversal state is separate from graph storage
- BFS marks visited at enqueue time, preventing duplicate frontier entries
- DFS is iterative and marks visited at first push
- parent/depth are assigned only on first discovery
- source has parent SIZE_MAX and depth 0
- unvisited vertices retain SIZE_MAX parent/depth sentinels
- parent edge is validated against the actual graph
- BFS exact parent/order is not treated as representation-independent
- cross-representation tests compare reachable sets and BFS shortest depths
- DFS tests compare reachability, not a brittle exact visitation order

Complexity:
- Adj List traversal O(V+E)
- Matrix traversal O(V^2)
- Chapter 039 Edge List neighbor enumeration can produce O(VE)
- O(V+E) is not claimed independently of representation

## Chapter 041 Review

Checked:
- outgoing vectors strictly sorted and duplicate-free
- cached indegree updated exactly once per successful mutation
- add u->v rejects self-loop, duplicate and any case where v already reaches u
- failed cycle check does not partially mutate the DAG
- remove cannot create a cycle
- Kahn uses an indegree copy rather than corrupting cached metadata
- validator recomputes indegrees, edge count and acyclicity
- randomized differential test independently checks reverse reachability in a boolean matrix

Complexity:
- has_edge O(log outdegree)
- topological sort O(V+E)
- dynamic add is O(V+E) in this teaching implementation because of cycle prevention

## Chapter 042 Review

Checked:
- wrapper starts with Adjacency List
- promote threshold must exceed demote threshold
- hysteresis prevents rapid List/Matrix thrashing near one boundary
- conversion allocates and fully replays into a new backend before swapping
- failed conversion leaves old backend intact
- add/remove logical mutation remains successful even if best-effort optimization conversion fails
- undirected replay inserts each logical edge once using u < v
- edge weights survive conversions
- switch_count increments only after successful backend swap
- randomized tests verify all edge/weight semantics across backend changes

Complexity:
- normal operation depends on active backend
- a mutation that triggers conversion can pay up to Theta(V^2 + E)
- automatic density thresholds are explicitly teaching heuristics, not universal optima
- peak memory during conversion is acknowledged

## Testing Review

- GraphTraversal cross-representation BFS/DFS comparison over randomized graph and 60 sources
- DAG randomized mutation/reference workload: 15,000 operations
- AdaptiveGraph randomized differential: 20,000 operations in each directedness mode
- validators run throughout mutation-heavy tests

## Definition of Done

Batch 14 is complete only when Fedora CI configures, builds Chapters 001–042 under warnings + ASan/UBSan and passes the full CTest suite.
