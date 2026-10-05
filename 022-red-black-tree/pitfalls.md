# Pitfalls — Red-Black Tree

- forgetting NULL is logically BLACK
- root left RED after fix-up
- red node with red child
- inconsistent black-height despite sorted inorder
- deletion losing x parent when x=NULL
- rotating sibling case in wrong direction
- recoloring near/far child incorrectly
- root deletion special case
- transplant parent-link bug
- checking only BST ordering in validator
