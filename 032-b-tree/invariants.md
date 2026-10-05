# Invariants — IntBTree

1. keys inside node strictly increasing
2. node key count <= 2t-1
3. non-root key count >= t-1
4. internal node has key_count+1 live children
5. separator ranges are globally correct
6. all leaves have same depth
7. unique keys
8. reachable key count == tree size
