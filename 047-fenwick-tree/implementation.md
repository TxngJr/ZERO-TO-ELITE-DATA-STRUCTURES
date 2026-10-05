# Implementation — IntFenwickTree

State:
- count
- int64_t tree[count+1]

Index 0 is unused.

API:
- create/free
- size
- point_add
- point_set
- point_get
- prefix_sum
- range_sum
- validate

Build is Theta(n) using parent propagation.
