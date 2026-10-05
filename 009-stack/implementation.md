# Implementation — Stack Variants

## ArrayStack

Representation:
- int *data
- size
- capacity

Growth:
- initial capacity 8
- geometric doubling
- size/capacity overflow checks

Operations:
- create/free
- push/pop/peek
- size/empty
- validate

## LinkedStack

Representation:
- top pointer
- size

Each node:
- value
- next

Push allocates one node.
Pop unlinks and frees one node.

## Monotonic algorithm

next_greater_values accepts:
- input
- n
- unresolved sentinel
- output

It uses an index stack because answers belong to original positions.

Each index is:
- pushed once
- popped at most once

## Failure handling

Container mutations return false on allocation failure.

next_greater_values returns false if temporary stack allocation fails. Caller should then treat output as incomplete.
