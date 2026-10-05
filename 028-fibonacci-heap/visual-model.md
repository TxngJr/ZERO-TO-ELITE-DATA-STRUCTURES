# Visual Model — Fibonacci Heap

Root list is circular:

    [3] <-> [8] <-> [17]
     ^                 |
     |_________________|

min -> 3

Example child list:

        8
      / | \
    12 15 21

children are also circular doubly linked.

## Cut

Before:

        8
       /
      12
     /
    5   <- decreased below parent

Cut 5:

roots:
    3 <-> 5 <-> 8 <-> ...

Parent 12 may become marked or be cut depending on prior mark state.
