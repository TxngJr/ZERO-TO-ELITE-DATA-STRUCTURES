# Invariants — Linked Lists

## Singly

Empty:
    size==0
    head==NULL
    tail==NULL

Non-empty:
    head!=NULL
    tail!=NULL
    tail->next==NULL

Traversal:
    exactly size nodes reachable from head
    last reachable node == tail

## Doubly

Empty endpoints as above.

Non-empty:
    head->prev==NULL
    tail->next==NULL

For each A->next=B:
    B->prev==A

Forward and backward traversals each visit exactly size nodes.

## Circular singly

Empty:
    size==0
    tail==NULL

Non-empty:
    tail!=NULL
    head=tail->next
    after exactly size next-steps from head, cursor returns to head
    no earlier return before size steps

## Mutation discipline

Insertion:
- allocate first
- establish links
- update endpoints
- increment size only after structure is connected

Erasure:
- identify victim
- reconnect neighbors
- update endpoints
- decrement size
- free victim after no list link points to it
