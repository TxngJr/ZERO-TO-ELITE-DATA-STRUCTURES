# Invariants — Lazy Segment Tree

For leaf:
    sum=min=logical leaf value represented at that node

For internal interval length L:
    sum = left.sum + right.sum + lazy*L
    min = min(left.min,right.min) + lazy

Push:
- applies parent lazy to children
- sets parent lazy to zero
- preserves logical array

Pull is valid only after parent lazy is zero.
