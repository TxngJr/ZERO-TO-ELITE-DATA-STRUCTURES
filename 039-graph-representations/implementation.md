# Implementation — GraphRepr

Kinds:
- GRAPH_REPR_EDGE_LIST
- GRAPH_REPR_ADJ_MATRIX
- GRAPH_REPR_ADJ_LIST

Common contract:
- fixed vertex count
- directed/undirected
- integer weight
- no self-loop
- no duplicate logical edge

APIs:
- add/remove/has
- neighbor visitor
- edge count
- allocated-byte estimate
- validator

Cross-representation tests replay identical operations and compare semantics.
