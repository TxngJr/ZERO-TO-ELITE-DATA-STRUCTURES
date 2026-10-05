# Invariants — Dynamic Segment Tree

1. root represents [domain_left,domain_right)
2. each recursive split uses mid=left+(right-left)/2
3. NULL subtree means all logical values zero
4. leaf node has no children
5. internal node sum = left.sum-or-zero + right.sum-or-zero
6. no allocated node is an empty zero leaf/path after successful pruning
7. node_count equals reachable allocated nodes
8. every updated coordinate lies inside domain
