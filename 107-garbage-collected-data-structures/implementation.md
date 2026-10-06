# Implementation Notes

`GcHeap` preallocates:
- object array
- free-index stack
- mark stack

No allocation occurs inside `gc_collect`, so collection itself cannot fail due to mark-stack allocation.

Each object stores:
- value
- two handles
- generation
- alive/marked/root flags

`gc_set_edge` validates child handles before publishing them, so live objects cannot intentionally contain stale outgoing references through public API.
