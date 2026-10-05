# Traversal Invariants

## Preorder stack

Every item in stack is a subtree root still awaiting visit.
Right is pushed before left so left is popped first.

## Inorder

Stack contains ancestors whose left path has been entered but whose node/right subtree has not been fully processed.

## Two-stack postorder

stack2 stores nodes in reverse of desired postorder construction.
Children are scheduled such that popping stack2 yields left-right-node.

## Level-order

Queue contains discovered but not yet visited nodes in nondecreasing depth order.

## General

Each reachable node must be output exactly once.
Output count must equal tree size.
Traversal must not mutate tree.
