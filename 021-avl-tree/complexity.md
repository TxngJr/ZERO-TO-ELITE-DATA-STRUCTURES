# Complexity — AVL Tree

AVL height:

    h=O(log n)

Therefore:

| Operation | Time |
|---|---:|
| contains | O(log n) |
| insert | O(log n) |
| remove | O(log n) |
| min/max concept | O(log n) |
| inorder | Theta(n) |
| validate | Theta(n) |

Rotation:
    Theta(1)

Recursive insert/remove auxiliary call stack:
    O(log n)

Storage:
    Theta(n)

Each node adds height metadata.
