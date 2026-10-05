# Theory — Graph Fundamentals

## Graph identity

A graph is defined by vertices and edges, not by the array/list/matrix used to store them.

This abstraction boundary is essential because Chapter 039 stores the same logical graph three ways.

## Handshaking lemma

Every undirected edge contributes exactly 1 to degree of each endpoint.

Therefore each edge contributes 2 to total degree:

    sum degree(v)=2E

Directed edge contributes:
- 1 to source out-degree
- 1 to destination in-degree

Therefore both sums equal E.

## Complete graph

Simple undirected K_n:

    E=n(n-1)/2

Complete directed graph without self-loops:

    E=n(n-1)

These bounds are useful for density calculations and matrix memory analysis.
