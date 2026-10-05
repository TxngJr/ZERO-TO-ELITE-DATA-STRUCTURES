# Pitfalls — Binary Tree

- confusing Binary Tree with BST
- forgetting child->parent update
- accepting a node from another tree
- freeing subtree before detaching parent link
- using a borrowed node after removal
- updating size by 1 when deleting multi-node subtree
- recursive destruction on extremely deep tree
- sharing one node as two children
- returning mutable raw struct and losing invariant control
- claiming add-child O(1) while defensive membership check is O(h)
