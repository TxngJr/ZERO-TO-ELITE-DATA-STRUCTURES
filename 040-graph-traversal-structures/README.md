# Chapter 040 — Graph Traversal Structures

## Goal

BFS และ DFS ไม่ได้ต้องการแค่ graph storage แต่ต้องมี **traversal state** ของตัวเอง:

- visited set
- frontier
- parent/predecessor
- depth
- discovery order
- temporary neighbor buffer

บทนี้แยก traversal workspace ออกจาก GraphRepr เพื่อให้ logical algorithm เดียวกันทำงานบน Edge List, Adjacency Matrix และ Adjacency List ได้.

## Traversal State vs Graph State

Graph stores:
- vertices
- edges
- weights

Traversal stores temporary state:
- which vertices were discovered
- how frontier evolves
- traversal tree
- source-relative depth

Traversal must not mutate graph structure.

## BFS Frontier

Breadth-First Search uses FIFO Queue.

Invariant:
vertices leave queue in nondecreasing BFS depth.

When discovering v from u first time:

    visited[v] = true
    parent[v] = u
    depth[v] = depth[u] + 1
    enqueue(v)

Mark-on-enqueue prevents duplicate queue entries.

For unweighted graphs, BFS depth is shortest path length in number of edges from source.

## DFS Frontier

Depth-First Search can use:
- recursive call stack
- explicit Stack

Teaching implementation uses an explicit stack to separate graph algorithm from process call-stack limits.

When a vertex is pushed for the first time:
- mark visited
- record parent
- record depth

DFS depth is traversal-tree depth, not shortest-path distance.

## Parent Array

Source:

    parent[source] = SIZE_MAX

Every other discovered vertex has one traversal-tree parent.

Following parent pointers from a visited vertex eventually reaches source.

This supports path reconstruction.

## Path Reconstruction

For BFS:

    target -> parent[target] -> ... -> source

Reverse that chain to obtain one shortest path.

For DFS the reconstructed path is a valid DFS-tree path, but not necessarily shortest.

## Order Array

order[] records vertices when removed from frontier for processing.

The exact order can depend on neighbor iteration order.

Therefore:
- do not assume BFS/DFS order is representation-independent
- reachable set is representation-independent
- BFS shortest distances are representation-independent

## Representation-Sensitive Complexity

With Adjacency List:
    BFS/DFS = O(V+E)

With Adjacency Matrix:
    every processed vertex scans V cells
    O(V^2)

With Chapter 039 Edge List:
    neighbor enumeration scans all E edges per processed vertex
    worst O(VE)

This is an important lesson:
same high-level traversal algorithm can have different runtime because graph representation changes neighbor-access cost.

## Workspace Reuse

GraphTraversal allocates arrays once for a fixed vertex count and can be reused across runs.

This avoids reallocating:
- visited
- parent
- depth
- order
- queue
- stack
- neighbor scratch buffer

before every traversal.

## Validation

Validator checks:
- workspace and graph vertex counts match
- order contains every visited vertex exactly once
- unvisited vertices keep sentinel parent/depth
- source has depth 0 and no parent
- every other visited vertex has a visited parent
- parent edge exists in graph
- depth[parent] + 1 == depth[child]

For BFS these tree depths are also shortest unweighted distances.

## Files

- src/graph_traversal.*
- tests/test_graph_traversal.c
- examples/traversal_demo.c
- benchmarks/traversal_repr_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
