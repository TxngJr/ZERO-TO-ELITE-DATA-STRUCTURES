# Chapter 018 — Tree Traversal

## Goal

Traversal คือการกำหนดลำดับที่เรา visit nodes. บทนี้สร้าง DFS แบบ Preorder/Inorder/Postorder ทั้ง recursive และ iterative รวมถึง BFS Level-order ด้วย Queue.

หลังจบบทนี้คุณควร:
- แยก DFS กับ BFS
- trace preorder/inorder/postorder
- เข้าใจ relation ระหว่าง recursion และ explicit stack
- implement iterative preorder
- implement iterative inorder
- implement iterative postorder
- implement level-order ด้วย queue
- วิเคราะห์ time Theta(n)
- วิเคราะห์ auxiliary space จาก height/width
- เข้าใจ traversal order use cases
- compare recursive/iterative correctness ผ่าน differential tests
- เข้าใจ stack-overflow risk ของ deep recursive traversal

## 1. Why Traversal Exists

Tree ไม่มี linear next element เดียว.

จาก node หนึ่งมีหลาย branches จึงต้องเลือก policy ว่า visit เมื่อไร.

DFS:
ลง branch ลึกก่อน.

BFS:
visit ตาม depth/level.

## 2. Preorder

Order:

    Node
    Left
    Right

Mnemonic:
    NLR

Tree:

        1
       / \
      2   3
     / \
    4   5

Preorder:

    1,2,4,5,3

Use cases:
- serialize tree shape with null markers
- copy structure
- prefix-like expression processing
- process parent before descendants

## 3. Inorder

Order:

    Left
    Node
    Right

Mnemonic:
    LNR

Same tree:

    4,2,5,1,3

For Binary Search Tree, inorder yields keys in sorted order under standard strict ordering invariant.

For arbitrary Binary Tree it has no sorting guarantee.

## 4. Postorder

Order:

    Left
    Right
    Node

Mnemonic:
    LRN

Same tree:

    4,5,2,3,1

Use cases:
- free children before parent
- evaluate expression tree bottom-up
- compute subtree aggregates

## 5. Level-order

Visit by depth:

    1,2,3,4,5

Uses Queue.

This is BFS on a tree.

## 6. Recursive DFS

Preorder:

    visit(node)
    recurse(left)
    recurse(right)

Inorder moves visit between recursive calls.
Postorder moves visit after recursive calls.

The runtime call stack stores pending work.

## 7. Iterative Preorder

Use explicit stack:

1. push root
2. pop node, visit
3. push right
4. push left

Push right first because stack is LIFO and left must be processed first.

## 8. Iterative Inorder

Maintain:
- cursor
- stack of ancestors

Algorithm:
- push cursor while going left
- when NULL, pop ancestor and visit
- move to its right subtree

Stack represents nodes whose left subtree is being/has been processed but node/right work remains.

## 9. Iterative Postorder

This chapter uses two explicit stacks for clarity:

1. push root to stack1
2. pop stack1 -> push to stack2
3. push children to stack1
4. pop stack2 to output

This reverses a root-right-left style order into left-right-root.

Alternative one-stack algorithms exist but are trickier.

## 10. Level-order Queue

1. enqueue root
2. dequeue node and visit
3. enqueue left then right
4. continue until queue empty

Maximum queue occupancy relates to tree width.

## 11. Time Complexity

All traversals visit each node exactly once:

    Theta(n)

assuming node access O(1).

Different orders do not change asymptotic visit count.

## 12. Recursive Space

DFS recursion depth:

    O(h)

Balanced:
    O(log n)

Skewed:
    O(n)

Thus recursive traversal of huge skewed tree can exhaust call stack.

## 13. Iterative DFS Space

Explicit stack:
    O(h) for preorder/inorder on typical binary tree traversal

Two-stack postorder implementation here:
    O(n) auxiliary memory worst-case.

One-stack variants can reduce constants/space pattern but are more complex.

## 14. BFS Space

Queue:
    O(w)

where w=maximum width.

Perfect tree can have last level about n/2 nodes, so worst:

    O(n)

## 15. Traversal Does Not Imply Search Efficiency

Visiting all nodes:
    Theta(n)

BST search can avoid subtrees using ordering invariant:
    O(h)

Traversal order alone does not create fast lookup.

## 16. Recursive vs Iterative Equivalence

Correct implementations should produce identical order.

Tests in this chapter compare:
- recursive preorder vs iterative preorder
- recursive inorder vs iterative inorder
- recursive postorder vs iterative postorder

This is differential testing across algorithm representations.

## 17. Visitor vs Output Array

Production traversal API may accept callback/iterator/generator.

Teaching API writes values into caller buffer because:
- order is easy to test
- ownership simple
- no callback complexity

Caller must provide capacity >= tree size.

## 18. Mutation During Traversal

These functions assume tree does not mutate during traversal.

Removing/relinking nodes while stack/queue holds borrowed node pointers can invalidate worklist entries.

Mutation-safe iterators require a stronger contract/design.

## Files

- src/tree_traversal.*
- tests/test_tree_traversal.c
- examples/traversal_demo.c
- benchmarks/traversal_benchmark.c
- theory/visual/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
