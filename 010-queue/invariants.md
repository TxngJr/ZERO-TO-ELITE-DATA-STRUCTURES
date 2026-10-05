# Invariants — Queue

## CircularQueue

1. size <= capacity
2. capacity==0 implies data==NULL and size==0
3. if capacity>0 then data!=NULL
4. if size>0 then head<capacity
5. logical item i maps to wrapped physical position from head
6. dequeue advances head by one modulo capacity
7. grow preserves logical sequence

## LinkedQueue

1. size==0 iff head==NULL and tail==NULL
2. non-empty implies head!=NULL and tail!=NULL
3. tail->next==NULL
4. exactly size nodes reachable from head
5. last reachable node==tail

## FIFO Behavior

After enqueue(x), x becomes newest/back item.

If queue before enqueue was sequence S:
    new queue = S followed by x

dequeue returns first element of S and leaves the suffix.
