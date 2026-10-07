# Chapter 123 — Graph Database Structures

บทนี้สร้าง property-graph storage model ที่แยก **authoritative records** ออกจาก **query indexes**.

โครงสร้างหลัก:
- node table
- node-ID hash index
- edge table
- outgoing CSR adjacency index
- incoming CSR adjacency index
- sorted label index
- BFS traversal บน outgoing CSR

## Nodes

Node มี:
- `uint64_t id`
- `uint32_t label`
- `int64_t property`

Node ID map ใช้ open addressing เพื่อ map external ID → dense node index.

## Edges

Edge มี:
- source dense index
- destination dense index
- edge type
- signed weight/property

Edges เก็บ append-only ใน authoritative edge array.

## Rebuildable Indexes

หลัง mutation, index version จะ stale.
`gdb_build_indexes` สร้าง:

```text
out_offsets + out_edge_ids
in_offsets  + in_edge_ids
label_order sorted by (label, node_id)
```

Queries ถูก reject จน index version ตรงกับ graph version เพื่อป้องกัน silently stale results.

## Tests

- 20,000 nodes
- 99,999 initial edges
- exact label cardinality 800
- outgoing/incoming typed neighbor checks
- type-1 chain shortest path = 999 hops
- stale-index rejection after mutation
- rebuild + full cross-index validation
- ASan/UBSan

## Complexity

Node lookup expected O(1).
Index rebuild O(V + E + V log V).
Neighbor scan O(degree).
BFS O(V + E) for selected edge type in worst case.
