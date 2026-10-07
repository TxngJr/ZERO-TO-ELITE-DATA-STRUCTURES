# Invariants

1. Node IDs are unique.
2. Dense node indices are 0..node_count-1.
3. Every hash-index entry maps to the correct dense node.
4. Every edge endpoint references a valid dense node.
5. out_offsets and in_offsets start at 0 and end at edge_count.
6. Every authoritative edge appears exactly once in outgoing CSR.
7. Every authoritative edge appears exactly once in incoming CSR.
8. Outgoing CSR places edge under its source node.
9. Incoming CSR places edge under its destination node.
10. Label index is a permutation of nodes sorted by (label,id).
11. Queries require index_version == graph_version.
