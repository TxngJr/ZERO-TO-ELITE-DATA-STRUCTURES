# Implementation — IntMinHeap

Representation:

    int *data
    size_t size
    size_t capacity

Features:
- geometric growth
- push
- peek
- pop
- bottom-up build from caller array
- arbitrary-index replace
- invariant validation

Build copies caller data; caller retains ownership.

Heap sort is separate and sorts the caller array in place using max-heap logic.
