# Implementation Notes

Graph capacities are fixed at creation time so the chapter can focus on indexing/query structure rather than allocator growth.

The node hash table is sized to at least twice node capacity and has no deletion/tombstones because node deletion is intentionally out of scope.

CSR indexes are rebuilt transactionally into temporary arrays; old indexes are replaced only after all allocations/build steps succeed.

Label index stores sortable `(label, id, node_index)` entries. Binary search finds the label range, then IDs are returned in deterministic node-ID order.

Validator cross-checks authoritative edges against both outgoing and incoming CSR indexes and validates label-index permutation.
