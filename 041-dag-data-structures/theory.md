# Theory — DAG

## Existence of a source

Assume a finite DAG has no source.

Then every vertex has an incoming edge.
Start at any vertex and repeatedly follow an incoming edge backward.

Because the graph is finite, some vertex must repeat, producing a directed cycle.

Contradiction.

Therefore every non-empty DAG has at least one source.

The sink proof is symmetric.

## Topological order

A directed graph has a topological ordering iff it is acyclic.

Kahn algorithm operationalizes this theorem by repeatedly removing zero-indegree vertices.

## Cycle-safe insertion

Adding u->v creates a cycle exactly when v already reaches u.

That gives a direct dynamic invariant check.
