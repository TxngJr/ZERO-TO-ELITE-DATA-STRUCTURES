# Pitfalls — Cartesian Tree

- confusing Cartesian Tree with Cartesian product
- claiming it is balanced
- using >= instead of > without documenting changed duplicate tie behavior
- breaking parent links when popped stack nodes become a new left child
- forgetting an old right child is exactly the subtree being reattached
- claiming direct tree RMQ is always O(log n)
- missing the RMQ/LCA connection
