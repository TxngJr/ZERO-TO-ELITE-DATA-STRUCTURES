# Implementation — IntDAG

Fields:
- vertex_count
- edge_count
- sorted outgoing vectors
- cached indegree[]

Operations:
- add edge with reachability-based cycle prevention
- remove edge
- edge lookup
- indegree/outdegree
- source/sink queries
- Kahn topological sort
- validator

The implementation prioritizes invariant clarity over advanced incremental topological-order algorithms.
