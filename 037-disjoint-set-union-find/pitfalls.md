# Pitfalls — DSU

- forgetting to initialize parent[i]=i
- attaching larger tree under smaller tree
- decrementing component count on redundant union
- reading non-root size as authoritative
- claiming worst-case constant time
- recursive find stack concerns for intentionally adversarial naive chains
- out-of-range indices
- expecting DSU to support delete/split
