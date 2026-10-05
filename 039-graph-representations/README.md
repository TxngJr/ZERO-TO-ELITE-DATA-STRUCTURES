# Chapter 039 — Graph Representations

## Goal

Logical graph เดียวกันสามารถเก็บได้หลาย representation.

บทนี้ implement 3 แบบภายใต้ API เดียว:

1. Edge List
2. Adjacency Matrix
3. Adjacency List

เพื่อเปรียบเทียบ:
- edge lookup
- neighbor iteration
- insertion/removal
- memory usage
- sparse vs dense graph

## Edge List

Store logical edges as endpoint records.

Space:
    Theta(E)

Strength:
- iterate all edges efficiently
- simple serialization
- natural for edge-centric algorithms

Weakness:
- has_edge O(E)
- neighbor enumeration O(E)

## Adjacency Matrix

Matrix M[V][V].

For undirected:

    M[u][v] == M[v][u]

Space:
    Theta(V^2)

Strength:
- edge existence Theta(1)
- weight lookup Theta(1)
- attractive for dense graphs

Weakness:
- large memory for sparse graphs
- neighbor iteration Theta(V)

## Adjacency List

Per vertex store neighbors.

Directed storage:
    Theta(V+E)

Undirected storage:
    Theta(V+2E)

This implementation keeps each neighbor vector sorted by destination.

Therefore:
- lookup O(log degree)
- insertion/removal may shift O(degree)
- neighbor iteration Theta(degree)

## One Logical Edge, Different Physical Entries

Undirected edge {u,v}:

Edge List:
    one canonical record

Matrix:
    two symmetric cells

Adjacency List:
    one arc in u list
    one arc in v list

Logical edge_count remains one.

## Directed Graph

Directed edge (u,v):
- edge list keeps ordered pair
- matrix sets only M[u][v]
- adjacency list stores only v under u

Reverse direction is a different logical edge.

## Neighbor Iteration

Unified visitor API enumerates outgoing/incident neighbors.

Algorithms should not depend on iteration order unless a contract explicitly guarantees it.

## Sparse vs Dense

Simple undirected max:

    V(V-1)/2

Directed without self-loops:

    V(V-1)

Sparse:
    E much smaller than V^2

Dense:
    E approaches V^2

Typical tendencies:
- sparse -> adjacency list
- dense/repeated edge lookup -> matrix
- edge-centric batch processing -> edge list

No universal winner because cache/layout/workload matter.

## Memory Model

Teaching matrix allocates:
- one byte presence per cell
- one int weight per cell

Adjacency List allocates per-vertex vector capacity.

Edge List allocates edge-array capacity.

memory_bytes reports teaching-structure allocations, excluding allocator metadata.

## Complexity Summary

| Operation | Edge List | Matrix | Adj List |
|---|---:|---:|---:|
| has_edge | O(E) | Theta(1) | O(log deg) |
| add edge | O(E) duplicate check | Theta(1) | O(deg) shifts |
| remove | O(E) | Theta(1) | O(deg) |
| neighbors(v) | O(E) | Theta(V) | Theta(deg(v)) |
| space | Theta(V+E) | Theta(V^2) | Theta(V+E) |

## Representation and Algorithms

BFS/DFS:
- adjacency list naturally supports O(V+E)
- matrix traversal often O(V^2)

Kruskal:
- edge list is natural

Floyd-Warshall:
- matrix is natural

Later chapters revisit these trade-offs in algorithm context.
