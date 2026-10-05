# Pitfalls — Tree Traversal

- pushing preorder children in wrong order
- visiting inorder node before fully descending left
- confusing postorder with reverse preorder without proving child order
- BFS using stack instead of queue
- forgetting empty tree case
- output buffer too small
- recursive traversal on enormous skewed tree
- claiming BFS auxiliary always O(h)
- claiming iterative always uses less memory
- mutating tree while worklist holds node pointers
- assuming inorder arbitrary tree is sorted
