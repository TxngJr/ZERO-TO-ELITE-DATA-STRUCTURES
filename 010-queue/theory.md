# Theory — Queue

## FIFO as an abstract law

Queue behavior depends on insertion age, not memory address.

Array and linked representations are observationally equivalent if they return the same sequence under the same enqueue/dequeue operations.

## Ring indexing

For capacity C:

    physical(i) = (head + i) mod C

When C is power of two, some implementations may replace modulo with bit masking, but that requires invariant C=2^k. This course implementation uses ordinary wrap logic for clarity.

## Full vs Empty ambiguity

If implementation stores only head and tail indices in a fixed ring, head==tail can mean either empty or full.

Solutions:
- keep size
- reserve one unused slot
- keep an extra full flag

Our implementation stores size, making:
    empty <=> size==0
    full <=> size==capacity

## Resizing

Copying physical memory directly is wrong when wrapped.

Need logical copy:

    new[i] = old[(head+i)%capacity]

for i in [0,size)

then head=0.

## Queue vs Deque

Queue exposes one insertion end and one removal end.

Deque exposes both ends. A deque can implement a queue, but a narrower Queue API can communicate stronger intent and reduce misuse.
