# Implementation — Queue Variants

## CircularQueue

Fields:
- int *data
- size_t capacity
- size_t head
- size_t size

No tail field is necessary:

    tail_insert = (head + size) % capacity

To avoid overflow in head+size, implementation uses a helper that wraps by comparing against capacity-head rather than relying on unchecked addition.

Growth:
- initial capacity 8
- geometric doubling
- logical-order copy
- head reset to 0

## LinkedQueue

Fields:
- head
- tail
- size

Enqueue allocates one node.
Dequeue unlinks/frees head.

## Validation

Both expose validate() for educational testing.
The circular validator checks structural metadata; the linked validator walks nodes and checks endpoint consistency.

## API Contract

dequeue/peek:
- false when empty or invalid
- output untouched is not guaranteed by API on false

enqueue:
- false on invalid queue or allocation failure
