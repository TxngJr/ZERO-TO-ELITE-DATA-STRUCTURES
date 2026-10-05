# Chapter 041 — DAG Data Structures

## Goal

Directed Acyclic Graph (DAG) เป็น directed graph ที่ไม่มี directed cycle.

DAG สำคัญกับ:
- build systems
- task dependencies
- package dependencies
- scheduling
- data pipelines
- compiler dependency graphs
- topological execution order

บทนี้สร้าง IntDAG ที่รักษา acyclic invariant ตั้งแต่ mutation.

## Directed Acyclic Invariant

For every directed edge:

    u -> v

ไม่มี directed path จาก v กลับไป u.

ดังนั้นก่อนเพิ่ม edge ใหม่ u->v:
1. reject self-loop
2. reject duplicate
3. search whether v can already reach u
4. if yes, adding edge would create cycle -> reject
5. otherwise insert edge and increment indegree[v]

This is intentionally simple and correct, not the fastest fully dynamic DAG algorithm.

## Representation

Each vertex stores sorted outgoing neighbors.

DAG also caches:

    indegree[v]

Why cache indegree?
- source queries become O(1)
- Kahn topological sort can copy indegrees and decrement them
- dependency scheduling naturally uses remaining prerequisite counts

## Source and Sink

Source:
    indegree(v) == 0

Sink:
    outdegree(v) == 0

A non-empty DAG always has at least one source and at least one sink.

## Topological Order

A topological order lists every vertex so that for each edge:

    u -> v

u appears before v.

Topological order may not be unique.

## Kahn Algorithm

1. copy indegree[]
2. enqueue all zero-indegree vertices
3. dequeue u
4. append u to order
5. decrement indegree of each outgoing neighbor
6. when a neighbor becomes zero, enqueue it
7. success iff exactly V vertices are emitted

The queue here represents "currently ready dependencies".

## Cycle Detection Connection

If Kahn emits fewer than V vertices, the remaining subgraph contains a cycle.

IntDAG mutation is designed to prevent that state, so validator expects topological sort to emit all vertices.

## Cached Metadata

Whenever adding u->v:

    indegree[v]++

Whenever removing u->v:

    indegree[v]--

Cached metadata improves query speed but creates another invariant that validation must check.

## Dynamic Edge Addition Cost

Sorted outgoing insertion itself costs:

    O(outdegree(u))

because of array shifting.

But cycle prevention requires reachability search:

    O(V+E)

using adjacency lists.

Therefore teaching add-edge complexity:

    O(V+E)

This is a deliberate correctness-first dynamic DAG structure.

## Removal

Removing an edge cannot create a cycle.

So remove:
- binary-search outgoing list
- remove/shift
- decrement indegree destination

Cost depends on outdegree and shifting.

## Topological Sort Complexity

With adjacency lists:

    O(V+E)

Space:
    O(V)

for indegree copy and queue.

## Dependency Interpretation

If edge:

    A -> B

course convention means:
A must occur before B / B depends on completion of A.

Then:
- indegree[B] = number of prerequisites
- sources are immediately runnable tasks
- Kahn queue models ready work

Always state edge-direction convention in real dependency systems because some tools reverse it.

## DAG vs Tree

A DAG can have:
- multiple parents
- shared descendants
- multiple sources/sinks

A tree has exactly one parent per non-root node.

Do not force DAG dependencies into a tree model.

## DAG vs General Directed Graph

General directed graph may contain cycles.

DAG-specific algorithms can exploit:
- topological order
- no back-cycle
- dynamic programming over order
- finite dependency progression

## Validator

Checks:
1. endpoints valid
2. no self-loop
3. outgoing neighbors strictly sorted
4. no duplicate edges
5. cached indegrees match actual incoming edges
6. stored edge_count equals total outgoing arcs
7. Kahn topological sort visits all V vertices

## Files

- src/int_dag.*
- tests/test_dag.c
- examples/dag_demo.c
- benchmarks/topological_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
