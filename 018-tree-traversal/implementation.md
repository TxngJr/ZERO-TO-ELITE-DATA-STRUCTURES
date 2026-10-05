# Implementation — Traversals

## API

Each traversal receives:
- tree
- output int buffer
- capacity
- out_written

For empty tree:
- output may be NULL
- written=0
- traversal succeeds

For non-empty:
capacity must be >= tree size.

## Recursive Helpers

Helpers receive node, output and write index.

No allocation.
Auxiliary memory is runtime call stack O(h).

## Iterative Preorder

Allocates n node pointers.
Push right before left.

## Iterative Inorder

Allocates n node pointers.
Uses cursor + ancestor stack.

## Iterative Postorder

Uses two n-pointer stacks for clarity.
Both allocation sizes are overflow checked.

## Level-order

Uses n-pointer queue.
Because each tree node enters queue at most once, n slots is a simple safe upper bound.

A tighter specialized implementation could size/grow according to observed width.
