# Invariants — Deque

## IntDeque

1. size <= capacity
2. capacity==0 implies data==NULL, size==0, head==0
3. capacity>0 implies data!=NULL
4. head<capacity whenever capacity>0
5. logical sequence i maps by wrapped offset from head
6. empty state canonicalized to head=0

## Monotonic Maximum Queue

At processing index i:
- deque indices strictly increase from front to back
- all stored indices are inside current/potential window
- corresponding values are non-increasing
- every removed-back candidate is dominated by a newer value
- front index identifies maximum once window is complete
