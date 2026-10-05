# Theory — Stack

## ADT laws

A useful abstract specification:

    empty() produces S0
    is_empty(S0)=true
    peek(push(S,x))=x

and popping immediately after push removes the most recently pushed element.

Error behavior for pop(empty) must be part of API contract.

## Representation independence

If client code observes only:
- returned values
- size
- empty status

then array-backed and linked-backed implementations can satisfy the same Stack ADT.

If client depends on capacity or raw node addresses, abstraction leaks.

## Persistent/immutable stack preview

A functional stack can be modeled as:

    Stack = Empty | Node(value, rest)

push returns a new head sharing old rest.
pop returns rest.

This naturally supports persistence/immutability and will reappear in Chapters 092–094.

## Min-stack pattern

To support get_min in constant time, one design keeps:
- value stack
- auxiliary minimum stack

or stores current-min metadata with each entry.

This is a common pattern: augment a structure while preserving a stronger invariant.

## Monotonic stack is a discipline

Monotonic stack is usually not a different container type. It is an ordinary stack used under an order invariant to solve a family of problems efficiently.
