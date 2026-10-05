# Complexity — Priority Queue

Let n=size.

## Binary Heap

peek:
    Θ(1)

push:
    append backing slot plus sift-up
    O(log n), worst Θ(log n)

pop max:
    root replacement plus sift-down
    O(log n), worst Θ(log n)

validate:
    Θ(n)

space:
    Θ(capacity)

## Unsorted Reference

push:
    Θ(1) append

pop max:
    Θ(n) scan

This can be better if pushes dominate and pops are extremely rare.

## Sorted Array

push:
    Θ(n)

peek/pop from chosen end:
    Θ(1)

Useful when reads/extractions dominate and updates are rare.

## Operation mix matters

For p pushes and q pops:
different representations can win depending on p:q ratio, cache behavior and constants.

The data structure should be chosen from workload, not name recognition.
