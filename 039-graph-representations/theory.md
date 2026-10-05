# Theory — Graph Representations

Two representations are semantically equivalent when they encode the same:
- vertex set
- directedness
- logical edge set
- edge metadata

Physical duplication in an undirected matrix/list does not create extra logical edges.

Matrix cost depends on V rather than E.
Adjacency-list cost scales with stored arcs.
That is why density strongly influences representation choice.
