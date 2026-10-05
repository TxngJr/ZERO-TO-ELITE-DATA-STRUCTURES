# Visual Model — Binary Min Heap

Array:

    [2,5,3,9,7,8,4]

Tree:

          2
       /     \
      5       3
    /  \     / \
   9    7   8   4

Each parent <= children.

But:
    5 > 3

is fine because siblings/subtrees need not be sorted relative to each other.
