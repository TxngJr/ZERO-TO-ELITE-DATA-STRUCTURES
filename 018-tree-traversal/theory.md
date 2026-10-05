# Theory — Tree Traversal

## DFS as Work Scheduling

Recursive DFS hides a stack in function calls.

Iterative DFS makes pending nodes explicit.

Both are worklist algorithms.

## Visit Position Defines Order

Given skeleton:

    recurse(left)
    recurse(right)

Place visit:
- before both => preorder
- between => inorder
- after both => postorder

This is a useful mental model instead of memorizing three unrelated algorithms.

## BFS and Depth

When queue processes nodes in FIFO order and children are appended after their parent, all nodes at depth d are processed before nodes at depth d+1.

This is why BFS solves shortest-edge-distance problems in unweighted graphs later.

## Traversal as Fold

Postorder naturally computes:

    result(node)=combine(
        value(node),
        result(left),
        result(right)
    )

Examples:
- size
- height
- subtree sum
- expression evaluation

This links tree traversal to recursion and dynamic programming.
