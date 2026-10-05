# Theory — Graph Traversal Structures

## First-discovery tree

The first edge that discovers a vertex becomes its parent edge.

All discovered vertices except source therefore form a rooted traversal tree.

For BFS the tree is a shortest-path tree in edge-count metric.

For DFS it reflects depth-first exploration choices.

## BFS shortest-path argument

When BFS dequeues a vertex at depth d, all undiscovered vertices reachable by one edge are assigned d+1.

Because FIFO processes all depth d vertices before depth d+1 vertices, no later discovery can produce a shorter edge-count path.

## Representation independence

The set of vertices reachable from source depends on graph semantics, not storage layout.

Exact DFS tree and tie-breaking BFS parent can differ when neighbor iteration orders differ.
