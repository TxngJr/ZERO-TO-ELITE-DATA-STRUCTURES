# Complexity — BST

Let h=height, n=size.

Search/insert/delete/min/max/successor/predecessor:
    O(h)

Best/controlled shape:
    h=Theta(log n)

Worst skew:
    h=Theta(n)

Inorder:
    Theta(n)

Space:
    node storage Theta(n)
    recursive validator/free O(h) stack

Sorted insertion into plain BST is a canonical adversarial shape example.
