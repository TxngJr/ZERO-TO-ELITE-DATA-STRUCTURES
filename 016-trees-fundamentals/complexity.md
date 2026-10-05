# Complexity — Tree Fundamentals

Let:
- n = nodes
- h = height
- w = maximum width

Full traversal:
    Theta(n)

Root-to-leaf descent:
    O(h)

Balanced binary tree:
    h=O(log n)

Arbitrary/skewed:
    h can be Theta(n)

Recursive traversal auxiliary call depth:
    O(h)

Breadth-first traversal queue:
    O(w), worst O(n)

Do not replace h with log n without a shape/balance invariant.
