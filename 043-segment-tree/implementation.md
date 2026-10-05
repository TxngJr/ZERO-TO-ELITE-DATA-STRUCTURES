# Implementation — IntSegmentTree

State:
- n
- capacity ~4n
- sum[]
- min[]

API:
- create/free
- size
- point_set
- point_get
- range_sum
- range_min
- validate

Queries use [left,right).

The implementation does not expose internal node indices.
