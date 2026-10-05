# Visual Model — Binary Tree

## Pointers

             +-----+
             | 10  |
             +-----+
              /   \
             v     v
          +----+  +----+
          | 20 |  | 30 |
          +----+  +----+
             ^
             |
          parent

Every child points back to its parent.

## Tree object

    IntBinaryTree
    +-----------+
    | root -----+----> node 10
    | size = 3  |
    +-----------+

## Remove left subtree

Before:

        10
       /  \
      20  30
     / \
    40 50

remove node 20 subtree.

After:

        10
          \
           30

size decreases by 3.
