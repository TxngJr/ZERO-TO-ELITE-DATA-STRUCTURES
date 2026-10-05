# Complexity — Tree Traversal

Let:
- n=size
- h=height
- w=max width

| Traversal | Time | Auxiliary |
|---|---:|---:|
| recursive preorder | Theta(n) | O(h) call stack |
| recursive inorder | Theta(n) | O(h) |
| recursive postorder | Theta(n) | O(h) |
| iterative preorder | Theta(n) | O(h) typical/worst O(n) allocated here |
| iterative inorder | Theta(n) | O(h) typical/worst O(n) allocated here |
| iterative postorder two-stack | Theta(n) | O(n) |
| level-order | Theta(n) | O(w), implementation allocates O(n) |

Important distinction:
logical maximum occupancy and allocated buffer capacity may differ.
