# Pitfalls — B* Tree

- calling an ordinary B-Tree a B* Tree
- splitting immediately without trying sibling redistribution
- doing 1-to-2 split where 2-to-3 behavior is required
- forgetting parent separator participates in redistribution
- mixing order/max-keys/min-children conventions
- claiming every non-root node is always >=2/3 after root split without root exception
- reusing ordinary B-Tree deletion and silently violating B* occupancy
- claiming teaching rebuild deletion is O(log n)
