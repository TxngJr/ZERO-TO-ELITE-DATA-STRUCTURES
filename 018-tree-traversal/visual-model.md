# Visual Model — Traversal

Tree:

        1
      /   \
     2     3
    / \   / \
   4   5 6   7

Preorder NLR:
    1 2 4 5 3 6 7

Inorder LNR:
    4 2 5 1 6 3 7

Postorder LRN:
    4 5 2 6 7 3 1

Level-order:
    1 2 3 4 5 6 7

## Inorder explicit stack snapshot

Before visiting 4:

stack:
top -> 4
       2
       1

cursor=NULL

pop 4, visit, move right.
