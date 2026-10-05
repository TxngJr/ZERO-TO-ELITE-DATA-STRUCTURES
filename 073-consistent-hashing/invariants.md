# Invariants — Consistent Hashing

1. virtual_nodes > 0
2. each physical node has exactly virtual_nodes ring points
3. ring points remain sorted
4. physical node IDs are unique
5. point_count = node_count * virtual_nodes
