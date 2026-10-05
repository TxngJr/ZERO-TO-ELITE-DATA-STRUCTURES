# Complexity — Segment Tree

Build:
    Theta(n)

Point update:
    O(log n)

Range query:
    O(log n)

Storage:
    Theta(n)

A query does not scan every element in the interval.
It reuses cached aggregates from canonical intervals.
