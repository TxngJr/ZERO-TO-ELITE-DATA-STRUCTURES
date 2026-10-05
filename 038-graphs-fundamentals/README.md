# Chapter 038 — Graphs Fundamentals

## Goal

Graph เป็น abstraction สำหรับความสัมพันธ์ระหว่าง entities.

Formal model:

    G = (V, E)

V = vertices
E = edges

บทนี้แยก **logical graph** ออกจาก representation ก่อนเข้าสู่ Chapter 039.

Teaching implementation เป็น fixed-vertex **simple graph**:
- vertex IDs = 0..V-1
- directed หรือ undirected
- integer edge weight
- no self-loop
- no parallel duplicate edge
- edge-list baseline representation

## Directed vs Undirected

Undirected edge:

    {u,v}

ไม่มีทิศ.

Directed edge:

    (u,v)

จาก u ไป v.

สำหรับ undirected implementation เรา canonicalize:

    from = min(u,v)
    to   = max(u,v)

เพื่อให้ edge identity ชัด.

## Weighted vs Unweighted

Graph structure คือ endpoints.

Weight เป็น edge metadata เช่น:
- distance
- cost
- latency
- capacity

Implementation นี้เก็บ int weight แม้ algorithm chapters ภายหลังจะตีความต่างกัน.

## Simple Graph Contract

This chapter rejects:
- self-loops u->u
- duplicate edge between same ordered endpoints
- parallel edges

Multigraph/pseudograph เป็น valid graph models อื่น แต่ไม่ใช่ contract ของ implementation นี้.

## Degree

Undirected:

    degree(v) = number of incident edges

Handshaking Lemma:

    sum_v degree(v) = 2|E|

Directed:
- out-degree(v)
- in-degree(v)

Laws:

    sum out-degree = |E|
    sum in-degree  = |E|

## Adjacency

u and v are adjacent if an edge connects them.

Directed graph ต้องระบุ direction:
- outgoing neighbor
- incoming neighbor

## Walk, Trail, Path

Walk:
    vertices/edges may repeat

Trail:
    edges do not repeat

Path:
    typically vertices do not repeat

Terminology can vary slightly by text, so state convention.

## Cycle

Cycle is a closed path-like structure returning to starting vertex.

Directed cycle must follow edge directions.

Acyclic directed graph:
    DAG

DAGs become important in Chapter 041.

## Connectivity

Undirected:
u,v connected if a path exists.

Connected component:
maximal connected vertex subset.

Directed:
- weak connectivity ignores directions
- strong connectivity requires mutual directed reachability

Algorithms for traversal/components begin in Chapter 040.

## Isolated Vertex

A graph can contain vertices with no edges.

This matters:
edge list alone does not reveal isolated vertices unless vertex_count is stored separately.

Teaching graph stores vertex_count explicitly.

## Subgraph

H is subgraph of G if:
- V(H) subset of V(G)
- E(H) subset of E(G) with valid endpoints

Induced subgraph chooses a vertex subset and includes all edges among those vertices from G.

## Density

For simple undirected graph with V vertices:

    E_max = V(V-1)/2

Directed without self-loops:

    E_max = V(V-1)

Sparse:
    E much smaller than V^2

Dense:
    E approaches V^2

Density strongly influences representation choice.

## Edge-List Baseline

This chapter stores edges in a dynamic array.

Advantages:
- simple
- compact O(V + E) logical storage
- excellent for iterating every edge

Disadvantages:
- has_edge(u,v) is O(E)
- neighbor query requires scan
- degree computation by scan is O(E)

This intentionally motivates Chapter 039.

## Mutation Semantics

add_edge:
- validates vertices
- rejects self-loop
- rejects duplicate
- appends edge

remove_edge:
- find edge
- shift remaining array

Logical graph order does not depend on edge-array order.

## Graph Invariants

1. vertex IDs in range
2. no self-loops
3. no duplicate logical edges
4. undirected endpoints canonical
5. edge_count <= capacity
6. directed identity preserves order
7. stored vertex_count exists even for isolated vertices

## Complexity — Edge-list Baseline

| Operation | Complexity |
|---|---:|
| add edge | O(E) duplicate check + amortized append |
| remove edge | O(E) |
| has edge | O(E) |
| edge iteration | Theta(E) |
| degree | Theta(E) |
| storage | Theta(V + E) logical metadata |

## Files

- src/int_graph.*
- tests/test_graph.c
- examples/graph_demo.c
- benchmarks/edge_lookup_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
