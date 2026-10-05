# Complexity — Suffix Tree

Teaching naive compressed construction:
    O(n²) worst case

Structure size:
    O(n)

Pattern traversal:
    O(m*d) with linear outgoing-edge scan
    d <=257 here

Count:
    search cost only after leaf_count cache

Report:
    search + O(k) leaf output

Ukkonen linear construction is discussed but not falsely attributed to this implementation.
