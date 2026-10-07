# Invariants

Route trie:
1. root node index is 0.
2. every child index is NONE or inside node_count.
3. every allocated node is reachable from root.
4. route metadata may exist at any depth 0..32.
5. lookup returns deepest route encountered.

Flow table:
6. capacity is power-of-two and >= 8.
7. size equals USED entry count.
8. tombstone count equals TOMBSTONE entry count.
9. USED+tombstones <= capacity.
10. every USED key is retrievable through its probe chain.
