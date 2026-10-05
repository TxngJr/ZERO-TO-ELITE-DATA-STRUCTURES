# Implementation — GraphTraversal

Workspace fields:
- vertex_count
- visited[]
- parent[]
- depth[]
- order[]
- queue[]
- stack[]
- scratch_neighbors[]
- last_source
- last_mode
- has_run

Operations:
- BFS
- iterative DFS
- visited/parent/depth queries
- processing-order query
- path reconstruction
- validation

Neighbor collection uses Chapter 039 GraphRepr visitor API.
