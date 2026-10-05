# Implementation — Three Linked Lists

## IntSList

Metadata:
- head
- tail
- size

Operations:
- push_front/back
- pop_front/back
- get
- insert
- erase
- size
- validate

pop_back is Θ(n) because predecessor of tail must be found.

## IntDList

Metadata:
- head
- tail
- size

Each node:
- prev
- next

pop_back is Θ(1)
insert/erase at index still Θ(n) because lookup is required

Optimization:
node_at chooses traversal from head or tail depending on index, making traversal Θ(min(i,n-1-i)).

## IntCList

Representation:
- tail pointer
- size
- head = tail ? tail->next : NULL

Operations:
- push_back
- pop_front
- rotate_left
- get
- validate

rotate_left:
    tail = tail->next

This changes logical head in Θ(1), a natural circular-list operation.

## Validation

Every implementation exposes a validation function for educational tests.

Production libraries may not pay O(n) validation after every operation, but invariant checks are excellent during development/debug builds.
