# Implementation — IntDaryHeap

Fields:
- int *data
- size
- capacity
- arity d

Constructor rejects d<2.

Sift-down:
1. prove current node has first child without overflow
2. compute first child
3. compute how many live children exist
4. scan only that contiguous range
5. swap with smallest

Build copies caller input then bottom-up heapifies.
