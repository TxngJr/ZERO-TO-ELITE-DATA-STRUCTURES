# Invariants — Persistent Segment Tree

1. node index 0 is never a published tree node
2. every version root is a valid pool index for nonempty trees
3. published nodes are immutable
4. leaf sum=min=value
5. internal sum=left.sum+right.sum
6. internal min=min(left.min,right.min)
7. child indices are valid earlier/current pool entries
8. every version independently represents a valid Segment Tree
9. failed update does not publish a new version
10. old version query results never change after later updates
