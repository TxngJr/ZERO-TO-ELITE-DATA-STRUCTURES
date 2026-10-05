# Theory — Segment Tree

## Interval decomposition

A range query is decomposed into disjoint canonical node intervals.

Each selected node is fully covered, so its cached aggregate can be reused directly.

## Correctness by structural induction

Base:
a leaf stores exactly one array value.

Step:
if both children store correct aggregates for their intervals, combining them produces the correct aggregate for the parent interval.

Point update preserves the invariant because only ancestors of the modified leaf depend on that value.

## Why half-open intervals?

[left,right):
- length = right-left
- empty iff left==right
- adjacent ranges [a,b) and [b,c) do not overlap
- split boundaries compose naturally
