# Invariants — IntSegmentTree

1. root interval is [0,n)
2. every internal interval splits into two disjoint children
3. child intervals exactly cover parent interval
4. leaf sum=min=value
5. internal sum=left.sum+right.sum
6. internal min=min(left.min,right.min)
7. point update recomputes every ancestor on the changed path
8. query never returns data outside [left,right)
