# Implementation — IntGraph

Graph fields:
- vertex_count
- directed
- dynamic edge array
- edge_count/capacity

Edge:
- from
- to
- weight

Undirected edges are canonicalized as min/max endpoints.

This is a baseline logical-model implementation, not claimed as the best graph representation.
